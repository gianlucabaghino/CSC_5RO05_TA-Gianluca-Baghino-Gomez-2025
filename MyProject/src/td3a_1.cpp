#include <pthread.h>
#include <iostream>

// Fonction pour incrémenter une valeur tant que l'on ne demande pas de l'arrêter
void* incrementer(void* v_stop){
    volatile bool* p_stop = (volatile bool*) v_stop;  // Pointeur vers la variable de contrôle pour arrêter la boucle
    double counter = 0.0;  // Compteur qui va être incrémenté
    while (not *p_stop){  // Boucle qui continue tant que 'stop' est faux
        counter += 1.0;  // Incrémentation du compteur
    }
    std::cout << "Counter value: " << counter << std::endl;  // Affichage de la valeur finale du compteur
    return v_stop;  // Retourne la variable de contrôle (pour des raisons de type)
}

int main()
{
    volatile bool stop = false;  // Variable volatile pour contrôler l'arrêt du thread
    pthread_t incrementThread;  // Déclaration du thread

    // Création du thread qui exécutera la fonction 'incrementer'
    pthread_create(&incrementThread, nullptr, incrementer, (void*) &stop);

    // Boucle qui attend que l'utilisateur entre 's' pour arrêter
    for (char cmd = 'r'; cmd != 's'; std::cin >> cmd)  // Si l'utilisateur tape autre chose que 's', la boucle continue
        std::cout << "Type 's' to stop: " << std::flush;  // Demande à l'utilisateur de taper 's' pour arrêter

    stop = true;  // L'utilisateur a tapé 's', on demande d'arrêter le thread
    pthread_join(incrementThread, nullptr);  // Attend que le thread 'incrementThread' termine avant de continuer
}
