#include <iostream>
#include "TimespecUtils.h"
#include "Chrono.h"

int main()
{
    // Gestion simplifiée du temps Posix

    // Test des fonctions de conversion de temps
    timespec tsp1 = timespec_from_ms(2500.5); // 2500.5 ms
    std::cout << "2500.5 ms en secondes : " << timespec_to_ms(tsp1) << " ms" << std::endl;

    // Test de la fonction timespec_now() pour obtenir l'heure actuelle
    timespec current_time = timespec_now();
    std::cout << "Temps actuel : " << current_time.tv_sec << " secondes et " 
              << current_time.tv_nsec << " nanosecondes" << std::endl;

    // Test de timespec_negate() pour inverser le temps
    timespec neg_time = timespec_negate(tsp1);
    std::cout << "Temps négatif : " << neg_time.tv_sec << " secondes et "
              << neg_time.tv_nsec << " nanosecondes" << std::endl;

    // Test d'ajout et de soustraction de temps
    timespec tsp2 = {3, 500000000}; // 3.5 secondes
    timespec added_time = timespec_add(tsp1, tsp2); // Addition des temps
    std::cout << "Temps additionné : " << added_time.tv_sec << " secondes et "
              << added_time.tv_nsec << " nanosecondes" << std::endl;

    timespec subtracted_time = timespec_subtract(tsp2, tsp1); // Soustraction des temps
    std::cout << "Temps soustrait : " << subtracted_time.tv_sec << " secondes et "
              << subtracted_time.tv_nsec << " nanosecondes" << std::endl;

    // Test des opérateurs + et -
    timespec sum = tsp1 + tsp2; // Somme des temps
    std::cout << "Somme des temps : " << sum.tv_sec << " secondes et "
              << sum.tv_nsec << " nanosecondes" << std::endl;

    timespec difference = tsp2 - tsp1; // Différence des temps
    std::cout << "Différence des temps : " << difference.tv_sec << " secondes et "
              << difference.tv_nsec << " nanosecondes" << std::endl;

    return 0;
}

