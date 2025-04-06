#include "LooperTD3.h"
#include <cfloat>

// Constructeur de la classe Looper
// Initialise le booléen d'arrêt à false et le compteur de boucles à 0.0
Looper::Looper() : m_doStop(false), m_iLoop(0.0) {}

// Méthode pour exécuter une boucle un certain nombre de fois (nLoops)
// La boucle s'arrête soit quand on atteint nLoops, soit si un arrêt est demandé (m_doStop)
double Looper::runLoop(double nLoops) {
    m_iLoop = 0.0;              // Réinitialisation du compteur
    m_doStop = false;           // Réinitialisation de la condition d'arrêt
    while (m_iLoop < nLoops && !m_doStop.load()) {
        m_iLoop += 1.0;         // Incrémentation du compteur à chaque itération
    }
    return m_iLoop;             // Retourne le nombre réel de boucles effectuées
}

// Méthode pour arrêter la boucle
// Change la variable d'arrêt et retourne la valeur actuelle du compteur
double Looper::stopLoop() {
    m_doStop.store(true);       // Demande l'arrêt de la boucle
    return m_iLoop;             // Retourne la dernière valeur atteinte
}

// Méthode pour obtenir l'échantillon actuel (valeur du compteur)
// Peut être appelée même si la boucle n'est pas terminée
double Looper::getSample() const {
    return m_iLoop;
}

