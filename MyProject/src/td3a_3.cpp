#include <pthread.h>
#include <iostream>

// Structure contenant les données partagées
struct Data {
    volatile bool stop;        // Booléen pour contrôler l'arrêt de l'incrémentation
    volatile double counter;    // Compteur à incrémenter
    pthread_mutex_t mutex;     // Mutex pour protéger l'accès au compteur
};

// Fonction exécutée par les threads pour incrémenter le compteur
void* incrementer(void* v_data){
    Data* p_data = (Data*) v_data; // Conversion du paramètre en un pointeur vers les données

    // Boucle jusqu'à ce que le flag 'stop' soit vrai
    while (not p_data->stop){
        pthread_mutex_lock(&p_data->mutex);  // Verrouiller le mutex avant de modifier le compteur
        p_data->counter += 1.0;              // Incrémenter le compteur
        pthread_mutex_unlock(&p_data->mutex); // Déverrouiller le mutex après modification
    }
    return v_data;  // Retourner les données
}

int main()
{
    // Initialisation des données et du mutex
    Data data = {false, 0.0, PTHREAD_MUTEX_INITIALIZER};
    
    // Tableau pour stocker les identifiants des threads
    pthread_t incrementThread[3];
    
    // Création des 3 threads qui exécuteront la fonction incrementer
    pthread_create(&incrementThread[0], nullptr, incrementer, (void*) &data);
    pthread_create(&incrementThread[1], nullptr, incrementer, (void*) &data);
    pthread_create(&incrementThread[2], nullptr, incrementer, (void*) &data);
    
    // Attente de l'entrée de l'utilisateur pour arrêter les threads
    for (char cmd = 'r'; cmd != 's'; std::cin >> cmd)
        std::cout << "Type 's' to stop: " << std::flush; // Demander à l'utilisateur de taper 's' pour arrêter
    
    data.stop = true;  // Arrêter l'incrémentation des threads

    // Attendre que tous les threads se terminent
    for (int i=0; i<3; i++) 
        pthread_join(incrementThread[i], nullptr);

    // Détruire le mutex une fois l'utilisation terminée
    pthread_mutex_destroy(&data.mutex);
    
    // Afficher la valeur finale du compteur
    std::cout << "Counter value: " << data.counter << std::endl;
}
