#include <iostream>
#include <csignal>
#include <ctime>
#include <unistd.h>

// Structure pour stocker les données du timer
struct TimerData {
    int counter = 0; // Compteur pour suivre le nombre d'incréments
};

// Fonction de gestionnaire de signal, appelée périodiquement par le timer
void myHandler(int, siginfo_t* si, void*) {
    TimerData* data = (TimerData*) si->si_value.sival_ptr; // Récupère les données
    ++data->counter;  // Incrémente le compteur
    std::cout << "Compteur: " << data->counter << std::endl; // Affiche la valeur du compteur
}

int main() {
    TimerData myData; // Déclare un objet pour le compteur local
    
    // Configuration du gestionnaire de signal pour SIGRTMIN
    struct sigaction sa;
    sa.sa_flags = SA_SIGINFO; // Utilisation de SIGINFO pour obtenir plus de détails
    sa.sa_sigaction = myHandler; // Lier le gestionnaire de signal
    sigemptyset(&sa.sa_mask); // Initialisation du masque de signaux
    if (sigaction(SIGRTMIN, &sa, nullptr) == -1) { // Vérifier l'initialisation
        std::cerr << "Erreur lors de la configuration du gestionnaire de signal !" << std::endl;
        return 1;
    }

    // Configuration du timer pour envoyer le signal SIGRTMIN
    struct sigevent sev;
    sev.sigev_notify = SIGEV_SIGNAL; // Notification par signal
    sev.sigev_signo = SIGRTMIN; // Signal SIGRTMIN
    sev.sigev_value.sival_ptr = (void*) &myData; // Passer l'adresse du compteur local
    
    timer_t timerID;
    if (timer_create(CLOCK_REALTIME, &sev, &timerID) == -1) { // Création du timer
        std::cerr << "Erreur lors de la création du timer !" << std::endl;
        return 1;
    }

    // Définir la période du timer (2 Hz = 500 ms)
    itimerspec its;
    its.it_value.tv_sec = 0;
    its.it_value.tv_nsec = 500000000; // 0,5 seconde (500 millions de nanosecondes)
    its.it_interval.tv_sec = 0;
    its.it_interval.tv_nsec = 500000000; // 0,5 seconde (500 millions de nanosecondes)

    if (timer_settime(timerID, 0, &its, nullptr) == -1) { // Configuration du timer
        std::cerr << "Erreur lors de la configuration du timer !" << std::endl;
        return 1;
    }

    // Affiche un message indiquant que le timer a démarré
    std::cout << "Timer démarré ! Appuyez sur Ctrl+C pour quitter manuellement si nécessaire." << std::endl;

    // Boucle principale qui attend le signal du timer
    // Cette boucle incrémente le compteur et s'arrête après 15 incréments
    while (myData.counter < 15) {
        pause();  // Attendre la réception du signal
    }

    // Une fois que le compteur atteint 15, affiche un message et quitte
    std::cout << "Le timer a été arrêté après 15 incrémentations !" << std::endl;

    // Supprimer le timer avant de quitter
    timer_delete(timerID);

    return 0;
}

