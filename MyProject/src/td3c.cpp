#include <iostream>
#include <unistd.h>
#include "Chrono.h"
#include "ThreadTD3.h"
#include "MutexTD3.h"

int main() {
    volatile bool stop = false;  // Déclare une variable volatile pour arrêter les threads
    volatile double counter = 0.0;  // Variable volatile pour suivre le compteur
    Mutex mutex;  // Utilisation de la classe Mutex au lieu de pthread_mutex_t

    // Définir la politique d'ordonnancement pour le thread principal
    Thread::setMainSched(SCHED_RR);

    // Création des threads d'incrémentation
    IncrementerThread t1(1, stop, counter, mutex);  // Premier thread
    IncrementerThread t2(2, stop, counter, mutex);  // Deuxième thread
    IncrementerThread t3(3, stop, counter, mutex);  // Troisième thread

    // Démarrage des threads
    t1.start();
    t2.start();
    t3.start();

    // Attente de l'entrée de l'utilisateur pour arrêter les threads
    for (char cmd = 'r'; cmd != 's'; std::cin >> cmd) {
        std::cout << "Type 's' to stop: " << std::flush;  // Affiche un message demandant à l'utilisateur de taper 's' pour arrêter
    }
    stop = true;  // Change la variable stop pour arrêter les threads

    // Joindre les threads (attendre leur terminaison)
    t1.join();
    t2.join();
    t3.join();

    // Affichage de la valeur finale du compteur
    std::cout << "Counter value: " << counter << std::endl;

    // Affichage du temps d'exécution des threads
    std::cout << "Thread 1 execution time: " << t1.duration_ms() << " ms" << std::endl;
    std::cout << "Thread 2 execution time: " << t2.duration_ms() << " ms" << std::endl;
    std::cout << "Thread 3 execution time: " << t3.duration_ms() << " ms" << std::endl;

    return 0;
}





