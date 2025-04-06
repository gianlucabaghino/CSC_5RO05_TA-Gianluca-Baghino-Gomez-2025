#include <iostream>
#include <vector>
#include <atomic>
#include <csignal>  // Gestion des signaux
#include <ctime>    // Gestion du temps
#include <stdexcept>
#include <cfloat>   // Ajouté pour DBL_MAX
#include "Chrono.h"
#include "TimespecUtils.h"

// Classe Timer pour la gestion des minuteries avec signaux
class Timer {
protected:
    timer_t m_timerId;  // Identifiant du timer
    bool m_isRunning;   // Indique si le timer est actif

public:
    Timer();  // Constructeur
    virtual ~Timer(); // Destructeur

    void start(timespec duration, bool periodic); // Démarrage du timer
    void start_ms(double milliseconds, bool periodic); // Démarrage avec une durée en millisecondes
    void stop(); // Arrêt du timer
    
    virtual void callback() = 0; // Méthode virtuelle pure appelée à chaque expiration

    static void call_callback(int signo, siginfo_t* info, void* context); // Gestion des signaux du timer
    static timespec timespec_from_ms(double milliseconds); // Conversion d'un temps en millisecondes vers timespec
};

// Implémentation de Timer
Timer::Timer() : m_isRunning(false) {
    struct sigaction sa;
    sa.sa_flags = SA_SIGINFO;
    sa.sa_sigaction = Timer::call_callback;
    sigemptyset(&sa.sa_mask);
    ::sigaction(SIGRTMIN, &sa, nullptr);

    struct sigevent sev;
    sev.sigev_notify = SIGEV_SIGNAL;
    sev.sigev_signo = SIGRTMIN;
    sev.sigev_value.sival_ptr = this;
    
    if(::timer_create(CLOCK_REALTIME, &sev, &m_timerId) == -1) {
        throw std::runtime_error("Erreur création timer");
    }
}

Timer::~Timer() {
    if(m_isRunning) stop(); // S'assurer que le timer est arrêté avant suppression
    ::timer_delete(m_timerId);
}

void Timer::start(timespec duration, bool periodic) {
    if(m_isRunning) stop(); // Arrêter le timer s'il est déjà en cours
    
    struct itimerspec its;
    its.it_value = duration;
    its.it_interval = periodic ? duration : timespec{0, 0};
    
    if(::timer_settime(m_timerId, 0, &its, NULL) == -1) {
        throw std::runtime_error("Erreur démarrage timer");
    }
    m_isRunning = true;
}

void Timer::start_ms(double milliseconds, bool periodic) {
    start(timespec_from_ms(milliseconds), periodic);
}

void Timer::stop() {
    struct itimerspec its = {};
    ::timer_settime(m_timerId, 0, &its, NULL);
    m_isRunning = false;
}

void Timer::call_callback(int, siginfo_t* info, void*) {
    Timer* timer = static_cast<Timer*>(info->si_value.sival_ptr);
    if(timer) timer->callback();
}

timespec Timer::timespec_from_ms(double milliseconds) {
    timespec ts;
    ts.tv_sec = static_cast<time_t>(milliseconds / 1000);
    ts.tv_nsec = static_cast<long>((milliseconds - ts.tv_sec * 1000) * 1000000);
    return ts;
}

// Classe Looper pour exécuter une boucle de calcul intensive
class Looper {
private:
    std::atomic<bool> m_doStop; // Indicateur pour arrêter la boucle
    double m_iLoop; // Nombre d'itérations effectuées

public:
    Looper() : m_doStop(false), m_iLoop(0.0) {}

    double runLoop(double nLoops = DBL_MAX) { // Utilisation de DBL_MAX pour itérations illimitées
        m_iLoop = 0.0;
        m_doStop = false;
        while(m_iLoop < nLoops && !m_doStop.load()) {
            m_iLoop += 1.0;
        }
        return m_iLoop;
    }

    double stopLoop() {
        m_doStop.store(true);
        return m_iLoop;
    }

    double getSample() const { return m_iLoop; }
};

// Classe Calibrator
class Calibrator : public Timer {
public:
    Calibrator(double samplingPeriod_ms, unsigned int nSamples)
    : m_samplingPeriod_ms(samplingPeriod_ms), m_nSamples(nSamples) {
        timespec ts = Timer::timespec_from_ms(m_samplingPeriod_ms);
        start(ts, true);
        m_looper.runLoop(DBL_MAX);
        stop();
        computeRegression();
    }

    void callback() {
        static double lastSample = 0;
        double current = m_looper.getSample();
        m_samples.push_back(current - lastSample);
        lastSample = current;
        
        if(m_samples.size() >= m_nSamples) {
            m_looper.stopLoop();
        }
    }

    double nLoops(double t) const { 
        return m_a * t; // Prédiction du nombre de boucles pour un temps donné
    }

    double getA() const { return m_a; }
    double getB() const { return m_b; }

private:
    void computeRegression() {
        double sumY = 0;
        const double interval_s = m_samplingPeriod_ms / 1000.0; // Convert ms to seconds
        
        for(unsigned i = 0; i < m_samples.size(); ++i) {
            sumY += m_samples[i]; // Sum loops per interval
        }
        
        // loops/second = (total loops) / (total time)
        m_a = sumY / (m_samples.size() * interval_s);
        m_b = 0;
    }

    double m_samplingPeriod_ms;
    unsigned int m_nSamples;
    double m_a = 0, m_b = 0;
    Looper m_looper;
    std::vector<double> m_samples;
};

// Programme principal
int main() {
    const double samplingPeriod_ms = 200.0;
    const unsigned nSamples = 10;

    try {
        Calibrator calibrator(samplingPeriod_ms, nSamples);
        
        std::cout << "Résultats de la calibration :\n";
        std::cout << "a = " << calibrator.getA() 
                  << ", b = " << calibrator.getB() << "\n\n";

        for(double time = 0.5; time <= 10.0; time += 0.2) {
            Chrono chrono;
            double loops = calibrator.nLoops(time);
            
            Looper looper;
            chrono.restart();
            looper.runLoop(loops);
            double actualTime = timespec_to_ms(chrono.lap()) / 1000.0;

            std::cout << "Temps demandé : " << time << "s\n"
                      << "Boucles prédites : " << loops << "\n"
                      << "Durée réelle : " << actualTime << "s\n"
                      << "--------------------------------\n";
        }
    } catch(const std::exception& e) {
        std::cerr << "Erreur : " << e.what() << std::endl;
        return 1;
    }

    return 0;
}