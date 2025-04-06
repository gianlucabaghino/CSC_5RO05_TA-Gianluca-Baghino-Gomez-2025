#include "MutexTD3.h"
#include "TimespecUtils.h"  
#include <iostream>
#include <stdexcept>
#include <pthread.h>
#include <string>
#include <ctime>

// Implémentation de la classe TimeoutException

TimeoutException::TimeoutException(long timeout_ms)
    : timeout_ms(timeout_ms) {
    message = "Timeout après " + std::to_string(timeout_ms) + " ms";
}

const char* TimeoutException::what() const noexcept {
    return message.c_str();
}

// Implémentation de la classe Mutex

Mutex::Mutex() {
    if (pthread_mutex_init(&mutex, nullptr) != 0) {
        throw std::runtime_error("Erreur d'initialisation du mutex");
    }
}

Mutex::Mutex(bool isInversionSafe) {
    if (!isInversionSafe) {
        // Comportement standard (même chose que le constructeur par défaut)
        if (pthread_mutex_init(&mutex, nullptr) != 0) {
            throw std::runtime_error("Erreur d'initialisation du mutex");
        }
        return;
    }

    // Si isInversionSafe est activé
    pthread_mutexattr_t attr;

    if (pthread_mutexattr_init(&attr) != 0) {
        throw std::runtime_error("Erreur d'initialisation des attributs du mutex");
    }

    // Définir le type récursif
    if (pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE) != 0) {
        pthread_mutexattr_destroy(&attr);
        throw std::runtime_error("Erreur de définition du type du mutex à récursif");
    }

    // Activer l’héritage de priorité
    if (pthread_mutexattr_setprotocol(&attr, PTHREAD_PRIO_INHERIT) != 0) {
        pthread_mutexattr_destroy(&attr);
        throw std::runtime_error("Erreur de définition du protocole du mutex à PTHREAD_PRIO_INHERIT");
    }

    // Initialiser le mutex avec les attributs
    if (pthread_mutex_init(&mutex, &attr) != 0) {
        pthread_mutexattr_destroy(&attr);
        throw std::runtime_error("Erreur d'initialisation du mutex avec les attributs");
    }

    pthread_mutexattr_destroy(&attr); // Nettoyer les attributs
}

Mutex::~Mutex() {
    pthread_mutex_destroy(&mutex);  // Détruire le mutex lors de la destruction de l'objet
}

// Implémentation de la classe Lock (imbriquée dans Mutex)

Mutex::Lock::Lock(Mutex& m) : mutex(m) {
    mutex.lock();  // Verrouiller immédiatement le mutex
}

Mutex::Lock::Lock(Mutex& m, double timeout_ms) : mutex(m) {
    // Essayer de verrouiller le mutex avec un délai, lance TimeoutException en cas d'échec
    if (!mutex.lock(timeout_ms)) {
        throw TimeoutException(timeout_ms);  // Passer timeout_ms à l'exception
    }
}

Mutex::Lock::~Lock() {
    mutex.unlock();  // Déverrouiller automatiquement le mutex lorsque l'objet Lock sort du scope
}

// Méthodes internes pour verrouiller et déverrouiller

void Mutex::lock() {
    if (pthread_mutex_lock(&mutex) != 0) {
        throw std::runtime_error("Erreur lors du verrouillage du mutex");
    }
}

bool Mutex::lock(double timeout_ms) {
    struct timespec timeout;
    clock_gettime(CLOCK_REALTIME, &timeout);  // Obtenir l'heure actuelle
    timeout = timespec_add_ms(timeout, timeout_ms);  // Ajouter la logique du délai en utilisant la fonction timespec_add_ms

    // Essayer de verrouiller le mutex avec un délai
    return pthread_mutex_timedlock(&mutex, &timeout) == 0;
}

void Mutex::unlock() {
    if (pthread_mutex_unlock(&mutex) != 0) {
        throw std::runtime_error("Erreur lors du déverrouillage du mutex");
    }
}


