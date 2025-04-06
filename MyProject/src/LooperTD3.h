#ifndef LOOPER_TD3_H
#define LOOPER_TD3_H

#include <atomic>

// Classe Looper : permet d’exécuter une boucle de calculs pour simuler une charge CPU
class Looper {
private:
    // Variable atomique utilisée pour arrêter la boucle de façon sûre entre threads
    std::atomic<bool> m_doStop;

    // Compteur pour enregistrer le nombre d’itérations effectuées
    double m_iLoop;

public:
    // Constructeur : initialise les membres
    Looper();

    // Lance une boucle de calculs pendant nLoops itérations ou jusqu’à l’arrêt
    double runLoop(double nLoops);

    // Demande l’arrêt de la boucle et retourne le nombre d’itérations effectuées
    double stopLoop();

    // Retourne la dernière valeur de m_iLoop (utilisée pour des mesures)
    double getSample() const;
};

#endif // LOOPER_TD3_H

