#include "ThreadTD3.h"
#include <iostream>
#include <stdexcept>
#include <pthread.h>
#include <unistd.h>

pthread_mutex_t resource = PTHREAD_MUTEX_INITIALIZER; // Mutex pour protéger la ressource partagée

// Implémentation de la classe Thread

Thread::Thread(int id) : id(id) {
    pthread_attr_init(&posixThreadAttId);  // Initialisation des attributs du thread
}

Thread::~Thread() {
    if (isStarted()) {
        pthread_join(posixThreadId, nullptr); // Attente de la fin du thread si déjà démarré
    }
    pthread_attr_destroy(&posixThreadAttId); // Destruction des attributs du thread
}

void Thread::start(int priority) {
    struct sched_param sch_param;
    sch_param.sched_priority = priority; // Définir la priorité du thread
    pthread_attr_setschedparam(&posixThreadAttId, &sch_param); // Appliquer la priorité aux attributs du thread

    int res = pthread_create(&posixThreadId, &posixThreadAttId, call_run, this);
    if (res != 0) {
        throw std::runtime_error("Error creating thread"); // Erreur lors de la création du thread
    }
}

void Thread::join() {
    if (pthread_join(posixThreadId, nullptr) != 0) {
        throw std::runtime_error("Error joining thread"); // Erreur lors de l'attente de la fin du thread
    }
}

double Thread::duration_ms() {
    return chrono.lap_ms(); // Retourne la durée écoulée en millisecondes
}

bool Thread::isStarted() const {
    return posixThreadId != 0; // Vérifie si le thread a été démarré
}

void* Thread::call_run(void* v_thread) {
    Thread* thread = static_cast<Thread*>(v_thread);

    thread->chrono.restart(); // Démarre le chronomètre
    thread->run();  // Appelle la méthode run() du thread
    thread->chrono.stop(); // Arrête le chronomètre
    
    return nullptr;
}

void Thread::setMainSched(int policy) {
    struct sched_param sch_param;
    sch_param.sched_priority = (policy == SCHED_RR || policy == SCHED_FIFO) ? sched_get_priority_max(policy) : 0;
    if (pthread_setschedparam(pthread_self(), policy, &sch_param) != 0) {
        throw std::runtime_error("Failed to set main thread scheduling policy"); // Erreur lors de la définition de la politique d'ordonnancement
    }
}

int Thread::getMainSched() {
    int policy;
    struct sched_param sch_param;
    pthread_getschedparam(pthread_self(), &policy, &sch_param); // Récupère la politique d'ordonnancement du thread principal
    return policy;
}

void Thread_A::run() {
    // Tâche A : Activée à t = 300
    std::cout << "Task A started" << std::endl;

    // Attendre 100 tics avant d'accéder à la ressource
    sleep(100);  // Simule l'attente de 100 tics

    pthread_mutex_lock(&resource); // Verrouille la ressource partagée
    std::cout << "Task A locked the resource" << std::endl;

    // Travail sur la ressource pendant 100 tics
    sleep(100);

    pthread_mutex_unlock(&resource); // Déverrouille la ressource partagée
    std::cout << "Task A released the resource" << std::endl;

    // Tâche A se termine après 400 tics
    sleep(200);
    std::cout << "Task A completed" << std::endl;
}

void Thread_B::run() {
    // Tâche B : Activée à t = 300
    std::cout << "Task B started" << std::endl;

    // Travail pendant 100 tics
    sleep(100);
    
    std::cout << "Task B completed" << std::endl;
}

void Thread_C::run() {
    // Tâche C : Activée à t = 0
    std::cout << "Task C started" << std::endl;

    // Attendre 200 tics avant d'accéder à la ressource
    sleep(200);

    pthread_mutex_lock(&resource); // Verrouille la ressource partagée
    std::cout << "Task C locked the resource" << std::endl;

    // Travail sur la ressource pendant 200 tics
    sleep(200);

    pthread_mutex_unlock(&resource); // Déverrouille la ressource partagée
    std::cout << "Task C released the resource" << std::endl;

    // Tâche C se termine après 500 tics
    sleep(100);
    std::cout << "Task C completed" << std::endl;
}

// Implémentation de la classe IncrementerThread

IncrementerThread::IncrementerThread(int id, volatile bool& stop, volatile double& counter, Mutex& mutex)
    : Thread(id), stop(stop), counter(counter), mutex(mutex) {}

void IncrementerThread::run() {
    while (!stop) {
        Mutex::Lock lock(mutex); // Verrouille le mutex avant d'incrémenter le compteur
        counter += 1.0;
    }
}

// Nouvelle implémentation de la classe Incrementer

Incrementer::Incrementer(Counter& counter, unsigned int nLoops, int id)
    : Thread(id), r_counter(counter), nLoops(nLoops) {}

void Incrementer::run() {
    for (unsigned int i = 0; i < nLoops; ++i) {
        r_counter.increment();  // Incrémente le compteur
    }
}


