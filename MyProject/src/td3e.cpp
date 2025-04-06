#include "TimerTD3.h"
#include "LooperTD3.h"
#include "CalibratorTD3.h"
#include "CpuLoopTD3.h"
#include "ThreadTD3.h"
#include "MutexTD3.h"
#include "Chrono.h"
#include "TimespecUtils.h"
#include "CounterTD3.h"
#include <iostream>
#include <pthread.h>
#include <string>

// Priorités des tâches
#define PRIORITY_A 20
#define PRIORITY_B 10
#define PRIORITY_C 5

extern pthread_mutex_t resource;  // Mutex partagé pour gérer l'accès à la ressource
Calibrator calibrator(10.0, 100);  // Création du calibrateur avec une période d'échantillonnage de 10 ms et 100 échantillons
CpuLoop cpuLoop_A(calibrator), cpuLoop_B(calibrator), cpuLoop_C(calibrator); // Instances distinctes de CpuLoop pour chaque tâche
Chrono chrono_A, chrono_B, chrono_C; // Chronos pour mesurer le temps d'exécution des tâches

// Déclarations des tâches (Thread_A, Thread_B, Thread_C)
Thread_A task_A(1); // Création de la tâche A avec l'ID 1
Thread_B task_B(2); // Création de la tâche B avec l'ID 2
Thread_C task_C(3); // Création de la tâche C avec l'ID 3

// Fonction pour convertir les tics en timespec (1 tic = 10 ms)
timespec timespec_from_ticks(int tics) {
    timespec ts;
    ts.tv_sec = (tics * 10) / 1000;  // Conversion des tics en secondes (1 tic = 10 ms)
    ts.tv_nsec = ((tics * 10) % 1000) * 1000000;  // Conversion des millisecondes restantes en nanosecondes
    return ts;
}

// Fonction de la tâche A
void* task_A_function(void* arg) {
    (void)arg;  // Suppression de l'avertissement pour le paramètre non utilisé
    chrono_A.restart();  // Redémarre le chrono pour la tâche A
    timespec start_time = timespec_from_ticks(300);  // Activation de A à t = 300 tics (3000 ms)
    calibrator.wait(start_time);  // Attend le bon moment pour démarrer la tâche

    // Exécution de la tâche A
    calibrator.start(start_time, true);  // Lance la tâche avec les arguments appropriés
    std::cout << "Task A started at time: " << chrono_A.lap_ms() << " ms" << std::endl;

    calibrator.wait(timespec_from_ticks(100));  // Attend 100 tics avant d'accéder à la ressource
    std::cout << "Task A waiting for the resource" << std::endl;
    pthread_mutex_lock(&resource);  // Demande l'accès à la ressource partagée
    std::cout << "Task A locked the resource at time: " << chrono_A.lap_ms() << " ms" << std::endl;

    // Utilise CpuLoop pour simuler le travail de la tâche
    cpuLoop_A.execute(100);  // Tâche A travaille pendant 100 tics avec la ressource
    pthread_mutex_unlock(&resource);  // Libère la ressource partagée
    std::cout << "Task A released the resource at time: " << chrono_A.lap_ms() << " ms" << std::endl;

    calibrator.wait(timespec_from_ticks(200));  // Tâche A termine après 400 tics d'exécution
    std::cout << "Task A completed at time: " << chrono_A.lap_ms() << " ms" << std::endl;

    std::cout << "Task A elapsed time: " << chrono_A.lap_ms() << " ms" << std::endl; // Affichage du temps écoulé pour A
    return NULL;
}

// Fonction de la tâche B
void* task_B_function(void* arg) {
    (void)arg;  // Suppression de l'avertissement pour le paramètre non utilisé
    chrono_B.restart();  // Redémarre le chrono pour la tâche B
    timespec start_time = timespec_from_ticks(300);  // Activation de B à t = 300 tics
    calibrator.wait(start_time);

    // Exécution de la tâche B
    calibrator.start(start_time, true);  // Lance la tâche avec les arguments appropriés
    std::cout << "Task B started at time: " << chrono_B.lap_ms() << " ms" << std::endl;

    calibrator.wait(timespec_from_ticks(100));  // Tâche B travaille pendant 100 tics
    std::cout << "Task B finished CPU work at time: " << chrono_B.lap_ms() << " ms" << std::endl;
    cpuLoop_B.execute(100);  // Tâche B effectue un travail CPU pendant 100 tics
    std::cout << "Task B completed at time: " << chrono_B.lap_ms() << " ms" << std::endl;

    std::cout << "Task B elapsed time: " << chrono_B.lap_ms() << " ms" << std::endl; // Affichage du temps écoulé pour B
    return NULL;
}

// Fonction de la tâche C
void* task_C_function(void* arg) {
    (void)arg;  // Suppression de l'avertissement pour le paramètre non utilisé
    chrono_C.restart();  // Redémarre le chrono pour la tâche C
    timespec start_time = timespec_from_ticks(0);  // Activation de C à t = 0 tics
    calibrator.wait(start_time);

    // Exécution de la tâche C
    calibrator.start(start_time, true);  // Lance la tâche avec les arguments appropriés
    std::cout << "Task C started at time: " << chrono_C.lap_ms() << " ms" << std::endl;

    calibrator.wait(timespec_from_ticks(200));  // Attend 200 tics avant de demander la ressource
    std::cout << "Task C waiting for the resource" << std::endl;
    pthread_mutex_lock(&resource);  // Demande l'accès à la ressource partagée
    std::cout << "Task C locked the resource at time: " << chrono_C.lap_ms() << " ms" << std::endl;

    // Utilise CpuLoop pour simuler le travail de la tâche
    cpuLoop_C.execute(200);  // Tâche C travaille pendant 200 tics avec la ressource
    pthread_mutex_unlock(&resource);  // Libère la ressource partagée
    std::cout << "Task C released the resource at time: " << chrono_C.lap_ms() << " ms" << std::endl;

    calibrator.wait(timespec_from_ticks(100));  // Tâche C termine après 500 tics d'exécution
    std::cout << "Task C completed at time: " << chrono_C.lap_ms() << " ms" << std::endl;

    std::cout << "Task C elapsed time: " << chrono_C.lap_ms() << " ms" << std::endl; // Affichage du temps écoulé pour C
    return NULL;
}

// Fonction d'initialisation du mutex avec ou sans héritage de priorité
void init_mutex(bool priority_inheritance) {
    pthread_mutexattr_t mattr;
    pthread_mutexattr_init(&mattr);
    if (priority_inheritance) {
        pthread_mutexattr_setprotocol(&mattr, PTHREAD_PRIO_INHERIT); // Protection contre l'inversion de priorité
    } else {
        pthread_mutexattr_setprotocol(&mattr, PTHREAD_PRIO_NONE); // Pas de protection contre l'inversion de priorité
    }
    pthread_mutex_init(&resource, &mattr);  // Initialisation du mutex avec les attributs définis
}

// Exécution sans protection contre l'inversion de priorité
void run_no_priority_inheritance() {
    std::cout << "Running with no priority inheritance protection (Pass 1)" << std::endl;
    init_mutex(false);  // Initialisation sans protection contre l'inversion de priorité
    
    // Création et configuration des tâches (identique au code actuel)
    pthread_t thread_A, thread_B, thread_C;
    struct sched_param param_A, param_B, param_C;

    // Définition des priorités des tâches
    param_A.sched_priority = PRIORITY_A;
    param_B.sched_priority = PRIORITY_B;
    param_C.sched_priority = PRIORITY_C;

    // Création des threads pour chaque tâche
    pthread_create(&thread_A, NULL, task_A_function, NULL);
    pthread_setschedparam(thread_A, SCHED_FIFO, &param_A);

    pthread_create(&thread_B, NULL, task_B_function, NULL);
    pthread_setschedparam(thread_B, SCHED_FIFO, &param_B);

    pthread_create(&thread_C, NULL, task_C_function, NULL);
    pthread_setschedparam(thread_C, SCHED_FIFO, &param_C);

    // Attente de la fin des tâches
    pthread_join(thread_A, NULL);
    pthread_join(thread_B, NULL);
    pthread_join(thread_C, NULL);

    // Nettoyage
    pthread_mutex_destroy(&resource);
}

// Exécution avec protection contre l'inversion de priorité
void run_with_priority_inheritance() {
    std::cout << "Running with priority inheritance protection (Pass 2)" << std::endl;
    init_mutex(true);  // Initialisation avec héritage de priorité activé
    
    // Création et configuration des tâches (identique au code actuel)
    pthread_t thread_A, thread_B, thread_C;
    struct sched_param param_A, param_B, param_C;

    // Définition des priorités des tâches
    param_A.sched_priority = PRIORITY_A;
    param_B.sched_priority = PRIORITY_B;
    param_C.sched_priority = PRIORITY_C;

    // Création des threads pour chaque tâche
    pthread_create(&thread_A, NULL, task_A_function, NULL);
    pthread_setschedparam(thread_A, SCHED_FIFO, &param_A);

    pthread_create(&thread_B, NULL, task_B_function, NULL);
    pthread_setschedparam(thread_B, SCHED_FIFO, &param_B);

    pthread_create(&thread_C, NULL, task_C_function, NULL);
    pthread_setschedparam(thread_C, SCHED_FIFO, &param_C);

    // Attente de la fin des tâches
    pthread_join(thread_A, NULL);
    pthread_join(thread_B, NULL);
    pthread_join(thread_C, NULL);

    // Nettoyage
    pthread_mutex_destroy(&resource);
}

int main(int argc, char *argv[]) {
    bool with_priority_inheritance = true; // Par défaut, protection activée

    if (argc > 1 && std::string(argv[1]) == "no-priority") {
        with_priority_inheritance = false; // Exécution sans protection si "no-priority" est passé en argument
    }

    // Choix du mode d'exécution en fonction du flag
    if (with_priority_inheritance) {
        run_with_priority_inheritance();
    } else {
        run_no_priority_inheritance();
    }

    return 0;
}








