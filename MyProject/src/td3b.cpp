#include <pthread.h>
#include <stdexcept>
#include <iostream>
#include "MutexTD3.h"  
#include <exception>
#include <string>
#include <vector>
#include <thread>

// Fonction pour ajouter un délai en millisecondes à un timespec (pour la compatibilité descendante)
struct timespec timespec_add_ms(const struct timespec& ts, long ms) {
    struct timespec result = ts;
    result.tv_sec += ms / 1000;  // Ajouter les secondes
    result.tv_nsec += (ms % 1000) * 1000000;  // Ajouter les nanosecondes
    if (result.tv_nsec >= 1000000000L) {
        result.tv_sec += 1;  // Si les nanosecondes dépassent 1 seconde, ajouter une seconde
        result.tv_nsec -= 1000000000L;  // Ajuster les nanosecondes
    }
    return result;
}

// Fonction à exécuter pour chaque thread
void task(Mutex& mtx, int& counter, double timeout_ms) {
    try {
        // Verrouiller le mutex avec un délai d'attente
        Mutex::Lock lock(mtx, timeout_ms);  // Verrouillage uniquement pour la section critique
        std::cout << "Thread a verrouillé le mutex, incrémentation du compteur" << std::endl;
        counter++;  // Incrémenter le compteur
    } catch (const TimeoutException& e) {
        // Gérer l'exception de délai d'attente
        std::cerr << "Exception de délai d'attente : " << e.what() << std::endl;
    } catch (const std::exception& e) {
        // Gérer les autres exceptions
        std::cerr << "Exception : " << e.what() << std::endl;
    }

    // Simuler un travail après avoir libéré le mutex
    std::this_thread::sleep_for(std::chrono::milliseconds(50));  // Travail simulé
}

int main() {
    Mutex mtx;  // Créer une instance de Mutex
    int counter = 0;  // Initialiser le compteur
    
    // Créer un vecteur pour contenir les threads
    std::vector<std::thread> threads;
    for (int i = 0; i < 100; ++i) {
        // Créer un thread avec un délai d'attente de 2000 ms pour verrouiller
        threads.emplace_back(task, std::ref(mtx), std::ref(counter), 2000);  // Délai de 2000 ms
    }

    // Attendre que tous les threads terminent
    for (auto& t : threads) {
        t.join();  // Attendre la fin de chaque thread
    }

    // Afficher la valeur finale du compteur
    std::cout << "Valeur finale du compteur : " << counter << std::endl;

    return 0;
}




