#include <iostream>
#include <vector>
#include <memory>
#include <stdexcept>
#include <functional>
#include <pthread.h>
#include <sched.h>
#include "Chrono.h"
#include "CounterTD3.h"  
#include "MutexTD3.h"
#include "ThreadTD3.h"
#include "TimespecUtils.h"

int main(int argc, char* argv[]) {
    // Vérification des arguments passés en ligne de commande
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " <nLoops> <nTasks> [protect] [schedPolicy]" << std::endl;
        return 1;
    }

    // Récupération des paramètres de la ligne de commande
    unsigned int nLoops = std::stoul(argv[1]);  // Nombre d'itérations par tâche
    unsigned int nTasks = std::stoul(argv[2]);  // Nombre de tâches à exécuter
    bool protect = (argc > 3) ? std::stoi(argv[3]) : false;  // Si protection des accès au compteur est activée
    int schedPolicy = (argc > 4) ? std::stoi(argv[4]) : SCHED_OTHER;  // Politique d'ordonnancement des threads

    // Vérification de la validité de la politique d'ordonnancement
    if (schedPolicy != SCHED_OTHER && schedPolicy != SCHED_RR && schedPolicy != SCHED_FIFO) {
        std::cerr << "Invalid scheduling policy! Use 0 (SCHED_OTHER), 1 (SCHED_RR), or 2 (SCHED_FIFO)." << std::endl;
        return 1;
    }

    // Création de l'objet Counter avec la protection si nécessaire
    Counter counter(protect);  // Utilisation de la classe Counter pour gérer les accès
    Chrono chrono;  // Chronomètre pour mesurer le temps d'exécution

    // Tableau de threads (tâches)
    std::vector<std::unique_ptr<Incrementer>> threads;  // Utilisation de la classe Incrementer pour les threads
    Mutex mutex;  // Mutex pour la synchronisation des threads

    // Création et démarrage des threads
    for (unsigned int i = 0; i < nTasks; ++i) {
        threads.emplace_back(std::make_unique<Incrementer>(counter, nLoops, i));  // On passe le Counter, nLoops, et l'ID du thread
    }

    chrono.restart();  // Redémarre le chronomètre pour mesurer le temps de début
    for (auto& thread : threads) {
        thread->start(schedPolicy);  // Démarre chaque thread d'Incrementer avec la politique d'ordonnancement choisie
    }
    for (auto& thread : threads) {
        thread->join();  // Attends que tous les threads aient terminé leur exécution
    }
    chrono.stop();  // Arrête le chronomètre

    // Calcul de l'erreur entre la valeur attendue et la valeur réelle
    double expected = nLoops * nTasks;  // La valeur attendue du compteur
    double actual = counter.value();  // Valeur réelle du compteur après l'exécution des threads
    double error = 100.0 * (1.0 - actual / expected);  // Calcul de l'erreur en pourcentage

    // Affichage des résultats
    std::cout << "Counter value: " << actual << " (Expected: " << expected << ", Error: " << error << "%)" << std::endl;
    std::cout << "Execution time: " << chrono.lap_ms() / 1000.0 << " seconds" << std::endl;

    return 0;
}





