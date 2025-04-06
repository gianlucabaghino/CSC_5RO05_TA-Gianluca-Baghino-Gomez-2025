#include "Semaphore.h"
#include <stdexcept>

// Constructeur de la classe Semaphore
// Initialise la valeur du compteur, la valeur maximale et les objets Mutex et Monitor
Semaphore::Semaphore(Mutex& m, unsigned long initValue, unsigned long maxValue)
    : counter(initValue), maxCount(maxValue), mutex(m), monitor(mutex) {}

// Fonction give() qui augmente le compteur du sémaphore
// Si le compteur est inférieur à la valeur maximale, on l'incrémente et on notifie un thread en attente
void Semaphore::give() {
    Mutex::Lock lock(mutex); // Verrouille le mutex pour accéder de manière sécurisée au sémaphore
    if (counter < maxCount) { // Vérifie si on peut augmenter le compteur
        ++counter; // Incrémente le compteur
        monitor.notify(); // Notifie un thread en attente
    }
}

// Fonction take() qui décrémente le compteur du sémaphore
// Attend que le compteur soit supérieur à 0 pour décrémenter
void Semaphore::take() {
    Mutex::Lock lock(mutex); // Verrouille le mutex pour accéder de manière sécurisée au sémaphore
    while (counter == 0) { // Tant que le compteur est à zéro, attendre
        // Attendre qu'un token soit disponible
        Monitor::Lock monitorLock(monitor);  // Verrouille le monitor pour gérer l'attente
        monitorLock.wait();  // Attend sans délai d'expiration
    }
    --counter; // Décrémente le compteur du sémaphore
}

// Fonction take(long timeout_ms) qui décrémente le compteur du sémaphore avec un délai d'attente
// Attend jusqu'à ce qu'un token soit disponible ou que le délai soit dépassé
void Semaphore::take(long timeout_ms) {
    Mutex::Lock lock(mutex); // Verrouille le mutex pour accéder de manière sécurisée au sémaphore
    while (counter == 0) { // Tant que le compteur est à zéro, attendre
        // Attendre avec un délai d'expiration
        Monitor::Lock monitorLock(monitor);  // Verrouille le monitor pour gérer l'attente
        monitorLock.wait(timeout_ms);  // Attend avec un délai d'attente spécifié
    }
    --counter; // Décrémente le compteur du sémaphore
}

// Fonction getCount() qui retourne la valeur actuelle du compteur
// Cette fonction est utilisée pour savoir combien de tokens sont disponibles dans le sémaphore
unsigned long Semaphore::getCount() const {
    return counter; // Retourne la valeur actuelle du compteur
}





