#include "CpuLoopTD3.h"

// Constructeur de la classe CpuLoop, prend une référence vers un calibrateur
CpuLoop::CpuLoop(Calibrator& calibrator)
    : m_calibrator(calibrator) {}

// Méthode qui exécute une boucle pour une durée donnée (en millisecondes)
// Retourne une paire : durée réelle (en secondes) et pourcentage d'erreur par rapport à la durée demandée
std::pair<double, double> CpuLoop::runTime(double duration_ms) {
    double duration_s = duration_ms / 1000.0; // Conversion en secondes
    double predictedLoops = m_calibrator.nLoops(duration_s); // Calcule le nombre de boucles à effectuer pour approximer la durée

    Looper looper;     // Objet Looper pour exécuter la boucle
    Chrono chrono;     // Chronomètre pour mesurer le temps réel d'exécution
    chrono.restart();  // Démarre le chronomètre
    looper.runLoop(predictedLoops); // Exécute la boucle
    double actual_s = timespec_to_ms(chrono.lap()) / 1000.0; // Convertit le temps écoulé en secondes
    double error = ((actual_s - duration_s) / duration_s) * 100.0; // Calcule l'erreur en pourcentage

    return {actual_s, error}; // Retourne la durée réelle et l'erreur
}

// Méthode qui simule une charge de travail CPU pendant un certain nombre de "ticks"
void CpuLoop::execute(int ticks) {
    // Simule du travail CPU pendant un nombre donné de ticks
    for (int i = 0; i < ticks; ++i) {
        // Calcul bidon pour occuper le processeur
        volatile int x = 0;
        for (int j = 0; j < 1000; ++j) {
            x += j;
        }
    }
}


