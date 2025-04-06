#include <pthread.h>
#include <iostream>

struct Data {
    volatile bool stop;     // Variable pour arrêter l'incrémentation
    volatile double counter; // Compteur à incrémenter
};

// Fonction pour incrémenter le compteur
void* incrementer(void* v_data){
    Data* p_data = (Data*) v_data;
    
    // Tant que 'stop' est faux, on incrémente le compteur
    while (not p_data->stop){
        p_data->counter += 1.0; // Incrémentation du compteur
    }
    return v_data; // Retour des données
}

int main()
{
    Data data = {false, 0.0}; // Initialisation des données avec stop = false et compteur = 0
    pthread_t incrementThread[3]; // Tableau de threads pour les 3 threads d'incrémentation

    // Création des trois threads
    pthread_create(&incrementThread[0], nullptr, incrementer, (void*) &data);
    pthread_create(&incrementThread[1], nullptr, incrementer, (void*) &data);
    pthread_create(&incrementThread[2], nullptr, incrementer, (void*) &data);

    // Demander à l'utilisateur de taper 's' pour arrêter
    for (char cmd = 'r'; cmd != 's'; std::cin >> cmd)
        std::cout << "Type 's' to stop: " << std::flush;  // Affichage de l'instruction pour l'utilisateur

    // Une fois 's' tapé, on arrête les threads
    data.stop = true;

    // Attente de la fin de chaque thread
    for (int i=0; i<3; i++) 
        pthread_join(incrementThread[i], nullptr);  // Joindre chaque thread pour qu'il se termine

    // Affichage de la valeur finale du compteur
    std::cout << "Counter value: " << data.counter << std::endl;
}
