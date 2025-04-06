#include <iostream>
#include <csignal>
#include <ctime>
#include <unistd.h>
#include <stdexcept>

// Classe abstraite Timer
class Timer {
public:
    // Constructeur
    Timer() : timerID(nullptr), isActive(false) {
        struct sigaction sa;
        sa.sa_flags = SA_SIGINFO;
        sa.sa_sigaction = &Timer::call_callback;
        sigemptyset(&sa.sa_mask);

        // Configuration du gestionnaire de signaux
        if (sigaction(SIGRTMIN, &sa, nullptr) == -1) {
            std::cerr << "Erreur lors de la configuration du gestionnaire de signaux !" << std::endl;
            throw std::runtime_error("Échec de la configuration du gestionnaire de signaux.");
        }

        struct sigevent sev;
        sev.sigev_notify = SIGEV_SIGNAL;
        sev.sigev_signo = SIGRTMIN;
        sev.sigev_value.sival_ptr = this;
        
        // Création du timer
        if (timer_create(CLOCK_REALTIME, &sev, &timerID) == -1) {
            std::cerr << "Erreur lors de la création du timer !" << std::endl;
            throw std::runtime_error("Échec de la création du timer.");
        }
    }

    // Destructeur
    virtual ~Timer() {
        if (timerID != nullptr) {
            timer_delete(timerID); // Suppression du timer
        }
    }

    // Fonction virtuelle de rappel
    virtual void callback() = 0;

    // Démarrer le timer avec une durée et une périodicité
    void start(const timespec &duration, bool periodic) {
        if (isActive) return; // Le timer est déjà actif

        itimerspec its;
        its.it_value = duration;
        its.it_interval = periodic ? duration : timespec{0, 0};

        // Configuration du timer
        if (timer_settime(timerID, 0, &its, nullptr) == -1) {
            std::cerr << "Erreur lors du réglage du timer !" << std::endl;
            throw std::runtime_error("Échec du démarrage du timer.");
        }

        isActive = true;
    }

    // Arrêter le timer
    void stop() {
        if (timerID == nullptr) return;

        itimerspec its{};
        its.it_value.tv_sec = 0;
        its.it_value.tv_nsec = 0;
        its.it_interval.tv_sec = 0;
        its.it_interval.tv_nsec = 0;

        if (timer_settime(timerID, 0, &its, nullptr) == -1) {
            throw std::runtime_error("Failed to stop timer.");
        }
        isActive = false;
    }

    // Vérifier si le timer est actif
    bool is_timer_active() const {
        return isActive;
    }

private:
    timer_t timerID; // Identifiant du timer
    bool isActive;   // État du timer

    // Fonction de rappel pour le signal
    static void call_callback(int, siginfo_t* si, void*) {
        Timer* timer = static_cast<Timer*>(si->si_value.sival_ptr);
        timer->callback();
    }
};

// Classe dérivée pour le compte à rebours
class CountDown : public Timer {
public:
    // Constructeur avec valeur de départ
    CountDown(int start) : counter(start) {}

    // Fonction de rappel pour chaque tick du timer
    void callback() override {
        if (counter < 0 && !isTimerStopped) {
            stop();  // Arrêter le timer lorsque le compte à rebours est terminé
            std::cout << "Compte à rebours terminé !" << std::endl;
            isTimerStopped = true;  // Empêcher les rappels futurs
        } else if (counter >= 0) {
            std::cout << "Compte à rebours : " << counter << std::endl;
            --counter;
        }
    }

private:
    int counter;                // Compteur du compte à rebours
    bool isTimerStopped = false; // Indicateur d'arrêt du timer
};

int main() {
    try {
        // Initialisation du compte à rebours
        CountDown countdown(10);
        timespec duration = {1, 0};  // Intervalle de 1 seconde
        countdown.start(duration, true);
        std::cout << "Compte à rebours démarré !" << std::endl;

        // Boucle d'attente pendant que le timer est actif
        while (countdown.is_timer_active()) {
            pause();  // Attente du signal
        }
    } catch (const std::exception &e) {
        std::cerr << "Erreur : " << e.what() << std::endl; // Gestion des erreurs
    }

    return 0;
}






