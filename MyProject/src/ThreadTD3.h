#ifndef THREADTD3_H
#define THREADTD3_H

#include <pthread.h>
#include <stdexcept>
#include "Chrono.h"
#include "MutexTD3.h"
#include "CounterTD3.h"

extern pthread_mutex_t resource;  // Cela permet aux autres fichiers d'accéder à 'resource'

// Classe abstraite Thread
class Thread {
protected:
    pthread_t posixThreadId;       // ID du thread POSIX
    pthread_attr_t posixThreadAttId;  // Attributs du thread POSIX
    const int id;                   // Identifiant unique du thread

    static void* call_run(void* v_thread); // Fonction statique pour appeler 'run' sur un thread

public:
    virtual ~Thread();  // Destructeur virtuel pour garantir la destruction correcte
    Thread(int id);  // Constructeur avec un identifiant pour le thread

    void start(int priority = 0);  // Démarre le thread avec une priorité (par défaut 0)
    void join();  // Attends la fin de l'exécution du thread
    virtual void run() = 0;  // Méthode virtuelle pure à implémenter dans les classes dérivées
    bool isStarted() const;  // Vérifie si le thread a démarré
    double duration_ms();  // Retourne la durée d'exécution en millisecondes

    static void setMainSched(int policy);  // Définit la politique d'ordonnancement pour le thread principal
    static int getMainSched();  // Récupère la politique d'ordonnancement du thread principal

private:
    Chrono chrono;  // Chronomètre pour mesurer la durée d'exécution du thread
};

// Classes dérivées pour des tâches spécifiques

class Thread_A : public Thread {
public:
    Thread_A(int id) : Thread(id) {}  // Constructeur qui initialise un thread avec un ID
    void run() override;  // Implémentation spécifique de la méthode run pour Thread_A
};

class Thread_B : public Thread {
public:
    Thread_B(int id) : Thread(id) {}  // Constructeur qui initialise un thread avec un ID
    void run() override;  // Implémentation spécifique de la méthode run pour Thread_B
};

class Thread_C : public Thread {
public:
    Thread_C(int id) : Thread(id) {}  // Constructeur qui initialise un thread avec un ID
    void run() override;  // Implémentation spécifique de la méthode run pour Thread_C
};

// Classe dérivée qui implémente la logique du thread pour l'incrémentation
class IncrementerThread : public Thread {
public:
    IncrementerThread(int id, volatile bool& stop, volatile double& counter, Mutex& mutex);  // Constructeur
    void run() override;  // Implémentation de la méthode run pour incrémenter un compteur

private:
    volatile bool& stop;  // Référence au flag pour arrêter le thread
    volatile double& counter;  // Référence au compteur à incrémenter
    Mutex& mutex;  // Mutex pour synchroniser l'accès au compteur
};

// Nouvelle classe Incrementer (qui imite l'ancien comportement)
class Incrementer : public Thread {
private:
    Counter& r_counter;  // Référence à un objet Counter
    const unsigned int nLoops;  // Nombre d'itérations à effectuer

public:
    Incrementer(Counter& counter, unsigned int nLoops, int id);  // Constructeur avec référence à Counter et nombre d'itérations
    void run() override;  // Implémentation de la méthode run pour l'incrémentation du compteur
};

#endif // THREADTD3_H







