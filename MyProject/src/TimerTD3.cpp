#include "TimerTD3.h"
#include <stdexcept>

// Constructeur du Timer
Timer::Timer() : m_isRunning(false) {
    struct sigaction sa;
    sa.sa_flags = SA_SIGINFO; // On utilise une fonction de rappel avec des infos supplémentaires
    sa.sa_sigaction = Timer::call_callback; // Fonction à appeler quand le signal est reçu
    sigemptyset(&sa.sa_mask);
    ::sigaction(SIGRTMIN, &sa, nullptr); // On associe SIGRTMIN à notre fonction de callback

    struct sigevent sev;
    sev.sigev_notify = SIGEV_SIGNAL; // On veut être notifié par un signal
    sev.sigev_signo = SIGRTMIN; // Le signal à utiliser
    sev.sigev_value.sival_ptr = this; // On passe un pointeur vers l'objet Timer

    // Création du timer avec les paramètres définis
    if (::timer_create(CLOCK_REALTIME, &sev, &m_timerId) == -1) {
        throw std::runtime_error("Erreur création timer");
    }
}

// Destructeur du Timer
Timer::~Timer() {
    if (m_isRunning) stop(); // Si le timer tourne encore, on l'arrête
    ::timer_delete(m_timerId); // On supprime le timer
}

// Démarrage du timer avec une durée donnée et possibilité de le rendre périodique
void Timer::start(timespec duration, bool periodic) {
    if (m_isRunning) stop(); // On arrête le timer s'il est déjà en cours

    struct itimerspec its;
    its.it_value = duration; // Temps avant le premier déclenchement
    its.it_interval = periodic ? duration : timespec{0, 0}; // Intervalle pour la périodicité

    // Démarrage du timer
    if (::timer_settime(m_timerId, 0, &its, nullptr) == -1) {
        throw std::runtime_error("Erreur démarrage timer");
    }
    m_isRunning = true;
}

// Démarrage du timer en spécifiant une durée en millisecondes
void Timer::start_ms(double milliseconds, bool periodic) {
    start(timespec_from_ms(milliseconds), periodic);
}

// Arrêt du timer
void Timer::stop() {
    struct itimerspec its = {}; // On met les valeurs à zéro pour désactiver le timer
    ::timer_settime(m_timerId, 0, &its, nullptr);
    m_isRunning = false;
}

// Fonction statique appelée quand le signal du timer est déclenché
void Timer::call_callback(int, siginfo_t* info, void*) {
    Timer* timer = static_cast<Timer*>(info->si_value.sival_ptr); // Récupération du pointeur vers le Timer
    if (timer) timer->callback(); // Appel de la méthode callback de l'objet
}

// Conversion d'un temps en millisecondes vers un objet timespec
timespec Timer::timespec_from_ms(double milliseconds) {
    timespec ts;
    ts.tv_sec = static_cast<time_t>(milliseconds / 1000); // Secondes entières
    ts.tv_nsec = static_cast<long>((milliseconds - ts.tv_sec * 1000) * 1e6); // Le reste en nanosecondes
    return ts;
}

