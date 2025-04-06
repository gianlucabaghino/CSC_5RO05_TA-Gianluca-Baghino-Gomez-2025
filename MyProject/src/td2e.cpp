#include <iostream>
#include <vector>
#include <atomic>
#include <csignal>
#include <ctime>
#include <stdexcept>
#include <cfloat>
#include <cmath>
#include "Chrono.h"
#include "TimespecUtils.h"

// Classe Timer permettant de gérer un timer POSIX
class Timer {
protected:
    timer_t m_timerId; // Identifiant du timer
    bool m_isRunning; // Indique si le timer est en cours d'exécution

public:
    Timer() : m_isRunning(false) {
        // Configuration du gestionnaire de signal pour le callback
        struct sigaction sa;
        sa.sa_flags = SA_SIGINFO;
        sa.sa_sigaction = Timer::call_callback;
        sigemptyset(&sa.sa_mask);
        ::sigaction(SIGRTMIN, &sa, nullptr);

        // Configuration de l'événement du timer
        struct sigevent sev;
        sev.sigev_notify = SIGEV_SIGNAL;
        sev.sigev_signo = SIGRTMIN;
        sev.sigev_value.sival_ptr = this;
        
        // Création du timer
        if(::timer_create(CLOCK_REALTIME, &sev, &m_timerId) == -1) {
            throw std::runtime_error("Erreur création timer");
        }
    }

    virtual ~Timer() {
        // Arrêt du timer si en cours d'exécution et suppression du timer
        if(m_isRunning) stop();
        ::timer_delete(m_timerId);
    }

    // Démarre le timer avec une durée et une périodicité
    void start(timespec duration, bool periodic) {
        if(m_isRunning) stop();
        
        struct itimerspec its;
        its.it_value = duration;
        its.it_interval = periodic ? duration : timespec{0, 0};
        
        // Lancement du timer
        if(::timer_settime(m_timerId, 0, &its, NULL) == -1) {
            throw std::runtime_error("Erreur démarrage timer");
        }
        m_isRunning = true;
    }

    // Démarre le timer en millisecondes avec périodicité
    void start_ms(double milliseconds, bool periodic) {
        start(timespec_from_ms(milliseconds), periodic);
    }

    // Arrête le timer
    void stop() {
        struct itimerspec its = {};
        ::timer_settime(m_timerId, 0, &its, NULL);
        m_isRunning = false;
    }

    // Fonction de callback, à définir dans les sous-classes
    virtual void callback() = 0;

    // Appelle la fonction de callback lorsque le signal est reçu
    static void call_callback(int, siginfo_t* info, void*) {
        Timer* timer = static_cast<Timer*>(info->si_value.sival_ptr);
        if(timer) timer->callback();
    }

    // Convertit un nombre de millisecondes en timespec
    static timespec timespec_from_ms(double milliseconds) {
        timespec ts;
        ts.tv_sec = static_cast<time_t>(milliseconds / 1000);
        ts.tv_nsec = static_cast<long>((milliseconds - ts.tv_sec * 1000) * 1000000);
        return ts;
    }
};

// Classe Looper qui exécute une boucle jusqu'à un arrêt demandé
class Looper {
private:
    std::atomic<bool> m_doStop; // Indicateur pour arrêter la boucle
    double m_iLoop; // Compteur de boucles exécutées

public:
    Looper() : m_doStop(false), m_iLoop(0.0) {}

    // Exécute la boucle jusqu'à un nombre spécifié de boucles ou un arrêt
    double runLoop(double nLoops = DBL_MAX) {
        m_iLoop = 0.0;
        m_doStop = false;
        while(m_iLoop < nLoops && !m_doStop.load()) {
            m_iLoop += 1.0;
        }
        return m_iLoop;
    }

    // Arrête la boucle en cours
    double stopLoop() {
        m_doStop.store(true);
        return m_iLoop;
    }

    // Retourne le nombre de boucles exécutées
    double getSample() const { return m_iLoop; }
};

// Classe Calibrator permettant d'estimer le nombre de boucles exécutables par seconde
class Calibrator : public Timer {
public:
    Calibrator(double samplingPeriod_ms, unsigned int nSamples)
    : m_samplingPeriod_ms(samplingPeriod_ms), m_nSamples(nSamples) {
        // Démarre le timer pour la calibration
        timespec ts = Timer::timespec_from_ms(m_samplingPeriod_ms);
        start(ts, true);
        m_looper.runLoop(DBL_MAX);
        stop();
        computeRegression();
    }

    // Callback appelé à chaque "tick" du timer pour enregistrer le temps de boucle
    void callback() override {
        static double lastSample = 0;
        double current = m_looper.getSample();
        m_samples.push_back(current - lastSample);
        lastSample = current;
        
        // Si le nombre d'échantillons est atteint, arrête la boucle
        if(m_samples.size() >= m_nSamples) {
            m_looper.stopLoop();
        }
    }

    // Calcule le nombre de boucles en fonction du temps
    double nLoops(double t) const { 
        return m_a * t;
    }

    // Retourne les paramètres de la régression
    double getA() const { return m_a; }
    double getB() const { return m_b; }

private:
    // Calcule la régression linéaire pour estimer les boucles par seconde
    void computeRegression() {
        double sumY = 0;
        const double interval_s = m_samplingPeriod_ms / 1000.0;
        
        for(unsigned i = 0; i < m_samples.size(); ++i) {
            sumY += m_samples[i];
        }
        
        m_a = sumY / (m_samples.size() * interval_s);
        m_b = 0;
    }

    double m_samplingPeriod_ms; // Période d'échantillonnage en millisecondes
    unsigned int m_nSamples; // Nombre d'échantillons à collecter
    double m_a = 0, m_b = 0; // Paramètres de la régression
    Looper m_looper; // Instance de Looper pour compter les boucles
    std::vector<double> m_samples; // Vecteur d'échantillons
};

// Classe CpuLoop qui exécute une boucle pendant un temps donné
class CpuLoop {
public:
    CpuLoop(Calibrator& calibrator) : m_calibrator(calibrator) {}

    // Exécute une boucle pendant un certain temps et mesure l'erreur par rapport au temps attendu
    std::pair<double, double> runTime(double duration_ms) {
        double duration_s = duration_ms / 1000.0;
        double predictedLoops = m_calibrator.nLoops(duration_s);

        Looper looper;
        Chrono chrono;
        chrono.restart();
        looper.runLoop(predictedLoops);
        double actual_s = timespec_to_ms(chrono.lap()) / 1000.0;
        double error = ((actual_s - duration_s) / duration_s) * 100.0;

        return {actual_s, error};
    }

private:
    Calibrator& m_calibrator; // Référence à l'instance de Calibrator
};
    
// Programme principal
int main() {
    const double samplingPeriod_ms = 200.0; // Période d'échantillonnage
    const unsigned nSamples = 10; // Nombre d'échantillons pour la calibration

    try {
        // Création d'un calibrateur et d'un CpuLoop
        Calibrator calibrator(samplingPeriod_ms, nSamples);
        CpuLoop cpuLoop(calibrator);
        
        // Affichage des résultats de la calibration
        std::cout << "Calibration réussie :\n";
        std::cout << "a = " << calibrator.getA() 
                    << ", b = " << calibrator.getB() << "\n\n";

        // Test des boucles pour différentes durées
        for(double duration_ms = 500; duration_ms <= 10000; duration_ms += 200) {
            auto [actual_s, error] = cpuLoop.runTime(duration_ms);
            double duration_s = duration_ms / 1000.0;
            
            // Affichage des résultats pour chaque durée demandée
            std::cout << "Demandé: " << duration_s << "s | "
                        << "Réel: " << actual_s << "s | "
                        << "Erreur: " << std::fabs(error) << "%\n";
        }
    } catch(const std::exception& e) {
        // Gestion des erreurs
        std::cerr << "Erreur : " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
