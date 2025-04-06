#include <iostream>
#include "Monitor.h"      
#include "MutexTD3.h"     
#include "TimespecUtils.h"
#include "ThreadTD3.h"    
#include "Chrono.h"       
#include "CounterTD3.h"


class NotifierThread : public Thread {
public:
    NotifierThread(Monitor& monitor, int id) 
        : Thread(id), monitor(monitor) {}  // On passe l'id au constructeur de Thread

protected:
    void run() override {
        // On peut simuler l'attente avec timespec_wait ici
        timespec delay = timespec_from_ms(2000); // 2 secondes en millisecondes
        timespec_wait(delay);  // Attente de 2 secondes

        monitor.notify();  // Envoi de la notification
        std::cout << "Notification envoyée !" << std::endl;
    }

private:
    Monitor& monitor;  // Référence à l'objet Monitor
};

int main() {
    try {
        Mutex mutex;  // Création du mutex
        Monitor monitor(mutex);  // Création du monitor avec le mutex

        std::cout << "Monitor initialisé avec succès !" << std::endl;

        // Création et démarrage du NotifierThread avec un id spécifique (par exemple, 1)
        NotifierThread notifier(monitor, 1);
        notifier.start();  // Démarrage du thread

        {
            Monitor::Lock lock(monitor);  // Verrouillage du monitor
            std::cout << "Monitor verrouillé..." << std::endl;
            // Vous pouvez ajouter plus de logique ici si nécessaire
        }

        notifier.join();  // Attente de la fin du thread NotifierThread

        std::cout << "Le thread principal est terminé !" << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Erreur : " << e.what() << std::endl;
    }

    return 0;
}









