#include <iostream>
#include "TimespecUtils.h"
#include "Chrono.h"

int main()
{
    // Gestion du temps avec Chrono

    Chrono chrono;  // Création d'un objet Chrono

    // Démarrer le chrono
    chrono.restart();
    std::cout << "Chrono démarré !" << std::endl;

    // Simuler du travail (attendre quelques secondes)
    timespec waitTime = timespec_from_ms(3000);  // Attendre 3 secondes
    timespec_wait(waitTime);

    // Prendre un tour
    timespec lapTime = chrono.lap();
    std::cout << "Temps du tour (secondes): " << lapTime.tv_sec << "s et " << lapTime.tv_nsec << "ns" << std::endl;

    // Arrêter le chrono
    timespec elapsedTime = chrono.stop();
    std::cout << "Temps écoulé (secondes): " << elapsedTime.tv_sec << "s et " << elapsedTime.tv_nsec << "ns" << std::endl;

    // Afficher le temps du tour en millisecondes
    double lapTimeMs = chrono.lap_ms();
    std::cout << "Temps du tour (ms): " << lapTimeMs << " ms" << std::endl;

    return 0;
}
