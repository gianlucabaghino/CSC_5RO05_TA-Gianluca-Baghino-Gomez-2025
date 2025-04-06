#include "Monitor.h"
#include <stdexcept>
#include <ctime>
#include <iostream>

// Constructeur de la classe Monitor, prend un mutex en paramètre
Monitor::Monitor(Mutex& m) : mutex(m) {
    // Initialisation de la variable de condition
    if (pthread_cond_init(&cond, nullptr) != 0) {
        throw std::runtime_error("Erreur d'initialisation de la condition variable");
    }
}

// Destructeur de la classe Monitor, détruit la variable de condition
Monitor::~Monitor() {
    if (pthread_cond_destroy(&cond) != 0) {
        std::cerr << "Erreur lors de la destruction de la condition variable" << std::endl;
    }
}

// Méthode pour notifier un seul thread en attente sur la condition variable
void Monitor::notify() {
    if (pthread_cond_signal(&cond) != 0) {
        throw std::runtime_error("Erreur lors de la notification d'un thread");
    }
}

// Méthode pour notifier tous les threads en attente sur la condition variable
void Monitor::notifyAll() {
    if (pthread_cond_broadcast(&cond) != 0) {
        throw std::runtime_error("Erreur lors de la notification de tous les threads");
    }
}

// Implémentation de la classe Lock

// Constructeur de la classe Lock qui verrouille le mutex et attache le monitor
Monitor::Lock::Lock(Monitor& m) : Mutex::Lock(m.mutex), monitor(m) {
    // Le mutex est verrouillé par le Lock parent
}

// Méthode pour faire attendre le thread sur la condition variable
void Monitor::Lock::wait() {
    // Utilisation de getMutex() pour accéder au mutex sous-jacent et passer à pthread_cond_wait
    if (pthread_cond_wait(&monitor.cond, &monitor.mutex.getMutex()) != 0) {
        throw std::runtime_error("Erreur lors de l'attente sur la condition variable");
    }
}

// Méthode pour faire attendre le thread sur la condition variable avec un timeout
void Monitor::Lock::wait(long timeout_ms) {
    struct timespec timeout;
    clock_gettime(CLOCK_REALTIME, &timeout);
    timeout.tv_sec += timeout_ms / 1000;  // Conversion des millisecondes en secondes
    timeout.tv_nsec += (timeout_ms % 1000) * 1000000;  // Conversion des millisecondes restantes en nanosecondes
    if (timeout.tv_nsec >= 1000000000) {  // Si les nanosecondes dépassent 1 seconde, ajuster
        timeout.tv_sec++;
        timeout.tv_nsec -= 1000000000;
    }

    // Attente sur la condition variable avec le timeout spécifié
    int res = pthread_cond_timedwait(&monitor.cond, &monitor.mutex.getMutex(), &timeout);
    if (res != 0 && res != ETIMEDOUT) {
        throw std::runtime_error("Erreur lors de l'attente avec timeout sur la condition variable");
    }
}



