#ifndef MUTEXTD3_H
#define MUTEXTD3_H

#include <pthread.h>
#include <stdexcept>
#include <exception>
#include <string>

// Classe d'exception pour un délai d'attente lors du verrouillage du mutex
class TimeoutException : public std::exception {
private:
    long timeout_ms;  // Délai d'attente en millisecondes
    std::string message;  // Message d'erreur

public:
    // Constructeur : initialise le message d'erreur avec le délai d'attente
    explicit TimeoutException(long timeout_ms);

    // Surcharge de la méthode what() pour retourner le message d'erreur
    const char* what() const noexcept override;
};

// Classe Mutex encapsulant un mutex POSIX avec RAII
class Mutex {
private:
    pthread_mutex_t mutex;  // Mutex POSIX

public:
    // Constructeur : initialise le mutex
    Mutex();

    // Constructeur : avec option pour l'héritage de priorité
    Mutex(bool isInversionSafe);

    // Destructeur : détruit le mutex
    ~Mutex();

    // Classe imbriquée Lock pour verrouiller et déverrouiller automatiquement le mutex
    class Lock {
    private:
        Mutex& mutex;  // Référence au mutex à verrouiller

    public:
        // Constructeur : verrouille immédiatement le mutex
        explicit Lock(Mutex& m);

        // Constructeur avec délai d'attente : essaie de verrouiller le mutex dans le délai donné
        Lock(Mutex& m, double timeout_ms);

        // Destructeur : déverrouille automatiquement le mutex
        ~Lock();
    };

    // Accesseur pour obtenir le mutex
    pthread_mutex_t& getMutex() { return mutex; }

private:
    // Fonction interne pour verrouiller le mutex
    void lock();

    // Fonction interne pour verrouiller le mutex avec un délai d'attente
    bool lock(double timeout_ms);

    // Fonction interne pour déverrouiller le mutex
    void unlock();
};

#endif // MUTEXTD3_H

