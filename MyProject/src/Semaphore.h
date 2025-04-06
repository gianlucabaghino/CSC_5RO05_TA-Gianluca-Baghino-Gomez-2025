#ifndef SEMAPHORE_H
#define SEMAPHORE_H

#include <climits>
#include "MutexTD3.h"
#include "Monitor.h"

// Classe Semaphore pour la gestion de sémaphores avec synchronisation
class Semaphore {
private:
    unsigned long counter;            // Compteur actuel du sémaphore (valeur disponible)
    const unsigned long maxCount;     // Valeur maximale que le compteur peut atteindre
    Mutex& mutex;                     // Référence au mutex utilisé pour protéger les accès concurrents
    Monitor monitor;                  // Monitor utilisé pour la synchronisation avec les threads

public:
    // Constructeur du sémaphore
    // initValue : valeur initiale du compteur
    // maxValue : valeur maximale du compteur
    Semaphore(Mutex& m, unsigned long initValue = 0, unsigned long maxValue = ULONG_MAX);

    // Fonction pour libérer une ressource (incrémenter le compteur)
    void give();

    // Fonction pour prendre une ressource (décrémenter le compteur), attente si aucune ressource disponible
    void take();

    // Fonction pour prendre une ressource avec un délai d'attente (timeout en millisecondes)
    void take(long timeout_ms);

    // Retourne la valeur actuelle du compteur
    unsigned long getCount() const;
};

#endif // SEMAPHORE_H





