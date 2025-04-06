#ifndef TIMER_TD3_H
#define TIMER_TD3_H

#include <ctime>
#include <csignal>

// Classe Timer : classe de base pour gérer un timer POSIX avec callback
class Timer {
protected:
    timer_t m_timerId;     // Identifiant du timer
    bool m_isRunning;      // Indique si le timer est en cours d'exécution

public:
    Timer();               // Constructeur
    virtual ~Timer();      // Destructeur virtuel

    // Démarre le timer avec une durée en timespec, peut être périodique
    void start(timespec duration, bool periodic);

    // Démarre le timer avec une durée en millisecondes, peut être périodique
    void start_ms(double milliseconds, bool periodic);

    // Arrête le timer
    void stop();

    // Méthode virtuelle pure appelée quand le timer expire
    virtual void callback() = 0;

    // Méthode statique appelée quand le signal du timer est reçu, redirige vers callback()
    static void call_callback(int, siginfo_t* info, void*);

    // Convertit une durée en millisecondes en structure timespec
    static timespec timespec_from_ms(double milliseconds);
};

#endif // TIMER_TD3_H

