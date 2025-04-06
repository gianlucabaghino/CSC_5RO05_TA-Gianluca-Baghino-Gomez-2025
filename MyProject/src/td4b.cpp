#include <iostream>
#include <vector>
#include "Semaphore.h"
#include "ThreadTD3.h"
#include "MutexTD3.h"

// Classe Thread Consumer
class TokenConsumer : public Thread {
private:
    Semaphore& semaphore;  // Référence à un sémaphore pour la gestion des tokens
    Mutex& printMutex;  // Mutex pour protéger l'affichage
    int id;  // Identifiant unique pour chaque consommateur
    int tokensConsumed = 0;  // Nombre de tokens consommés
    int tokensToConsume;  // Nombre total de tokens à consommer

public:
    TokenConsumer(Semaphore& sem, Mutex& printMutex, int id, int tokensToConsume) 
        : Thread(id), semaphore(sem), printMutex(printMutex), id(id), tokensToConsume(tokensToConsume) {}

    void run() override {
        // Boucle tant que le consommateur n'a pas consommé tous les tokens
        while (tokensConsumed < tokensToConsume) {
            semaphore.take();  // Cette opération bloque si aucun token n'est disponible
            
            // Verrouillage du mutex avant d'afficher pour garantir un affichage sûr en multithreading
            Mutex::Lock lock(printMutex);  // Verrouillage automatique du mutex
            
            std::cout << "Consumer " << id << " consumed a token." << std::endl;

            tokensConsumed++;  // Incrémenter le nombre de tokens consommés
        }
    }

    // Retourne le nombre de tokens consommés
    int getTokensConsumed() const {
        return tokensConsumed;
    }
};

// Classe Thread Producer
class TokenProducer : public Thread {
private:
    Semaphore& semaphore;  // Référence à un sémaphore pour la gestion des tokens
    Mutex& printMutex;  // Mutex pour protéger l'affichage
    int id;  // Identifiant unique pour chaque producteur
    int tokensToProduce;  // Nombre total de tokens à produire

public:
    TokenProducer(Semaphore& sem, Mutex& printMutex, int id, int tokens) 
        : Thread(id), semaphore(sem), printMutex(printMutex), id(id), tokensToProduce(tokens) {}

    void run() override {
        // Boucle pour produire les tokens
        for (int i = 0; i < tokensToProduce; ++i) {
            semaphore.give();  // Donne un token au sémaphore
            
            // Verrouillage du mutex avant d'afficher pour garantir un affichage sûr en multithreading
            Mutex::Lock lock(printMutex);  // Verrouillage automatique du mutex
            std::cout << "Producer " << id << " produced a token." << std::endl;
        }
    }
};

int main(int argc, char* argv[]) {
    // Vérification des arguments passés en ligne de commande
    if (argc != 4) {
        std::cerr << "Usage: " << argv[0] << " <num_consumers> <num_producers> <tokens_per_producer>" << std::endl;
        return 1;
    }

    // Récupération des paramètres
    int numConsumers = std::stoi(argv[1]);  // Nombre de consommateurs
    int numProducers = std::stoi(argv[2]);  // Nombre de producteurs
    int tokensPerProducer = std::stoi(argv[3]);  // Nombre de tokens par producteur

    try {
        Mutex printMutex;  // Mutex pour synchroniser les affichages
        Mutex resourceMutex;  // Mutex pour protéger le sémaphore
        Semaphore semaphore(resourceMutex);  // Sémaphore pour gérer la production et la consommation des tokens

        // Calcul du nombre total de tokens produits
        int totalTokens = tokensPerProducer * numProducers;

        // Calcul du nombre de tokens par consommateur
        int tokensPerConsumer = totalTokens / numConsumers;
        int remainingTokens = totalTokens % numConsumers;  // Calcul des tokens restants à distribuer

        // Création et démarrage des threads producteurs
        std::vector<TokenProducer*> producers;
        for (int i = 0; i < numProducers; ++i) {
            producers.push_back(new TokenProducer(semaphore, printMutex, i + 1, tokensPerProducer));
            producers[i]->start();  // Démarrage du thread producteur
        }

        // Création et démarrage des threads consommateurs
        std::vector<TokenConsumer*> consumers;
        for (int i = 0; i < numConsumers; ++i) {
            // Distribution des tokens restants parmi les consommateurs
            int tokensForThisConsumer = tokensPerConsumer + (i < remainingTokens ? 1 : 0);
            consumers.push_back(new TokenConsumer(semaphore, printMutex, i + 1, tokensForThisConsumer));
            consumers[i]->start();  // Démarrage du thread consommateur
        }

        // Attente que tous les producteurs finissent de produire
        for (int i = 0; i < numProducers; ++i) {
            producers[i]->join();  // Attente de la fin du thread producteur
            delete producers[i];  // Libération de la mémoire allouée au producteur
        }

        // Attente que tous les consommateurs finissent de consommer
        for (int i = 0; i < numConsumers; ++i) {
            consumers[i]->join();  // Attente de la fin du thread consommateur

            // Verrouillage du mutex pour afficher en toute sécurité
            Mutex::Lock lock(printMutex);  // Verrouillage automatique du mutex

            // Affichage du nombre de tokens consommés par chaque consommateur
            std::cout << "Consumer " << (i + 1) << " consumed " << consumers[i]->getTokensConsumed() << " tokens." << std::endl;

            delete consumers[i];  // Libération de la mémoire allouée au consommateur
        }

        // Fin de la simulation
        std::cout << "Simulation complete." << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;  // Affichage des erreurs éventuelles
    }

    return 0;  // Fin du programme
}







