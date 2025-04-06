#ifndef MONITOR_H
#define MONITOR_H

#include "MutexTD3.h"  
#include <pthread.h>
#include <stdexcept>

class Monitor {
private:
    pthread_cond_t cond;  // Condition variable POSIX, utilisée pour la synchronisation des threads
    Mutex& mutex;         // Mutex associé au monitor, utilisé pour protéger l'accès aux ressources partagées

public:
    // Constructeur: initialisation de la condition variable et association du mutex
    explicit Monitor(Mutex& m);

    // Destructeur: destruction de la condition variable, libère les ressources associées
    ~Monitor();

    // Méthode pour réveiller un seul thread qui attend sur la condition variable
    void notify();

    // Méthode pour réveiller tous les threads qui attendent sur la condition variable
    void notifyAll();

    // Classe Lock associée à Monitor, utilisée pour gérer le verrouillage du mutex lié à ce monitor
    class Lock : public Mutex::Lock {
    private:
        Monitor& monitor;  // Référence au monitor pour lequel le lock est pris

    public:
        // Constructeur qui bloque le mutex et prépare la condition variable
        explicit Lock(Monitor& monitor);

        // Méthode pour attendre qu'un signal soit reçu (condition met)
        void wait();

        // Méthode pour attendre avec un timeout (délai d'attente en millisecondes)
        void wait(long timeout_ms);
    };
};

#endif // MONITOR_H

