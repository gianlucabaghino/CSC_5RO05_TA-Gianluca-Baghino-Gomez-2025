#include "CounterTD3.h"
#include <pthread.h>

Counter::Counter(bool protect) : m_value(0.0) {
    if (protect) {
        // Initialiser le mutex si la protection est activée
        p_mutex = std::make_unique<pthread_mutex_t>();
        pthread_mutex_init(p_mutex.get(), nullptr);  // Initialiser le mutex
    } else {
        p_mutex = nullptr;  // Pas de mutex si la protection n'est pas activée
    }
}

Counter::~Counter() {
    if (p_mutex) {
        pthread_mutex_destroy(p_mutex.get());  // Détruire le mutex s'il a été initialisé
    }
}

void Counter::increment() {
    if (p_mutex) {
        pthread_mutex_lock(p_mutex.get());  // Verrouiller le mutex
    }

    m_value += 1.0;  // Incrémenter la valeur du compteur

    if (p_mutex) {
        pthread_mutex_unlock(p_mutex.get());  // Déverrouiller le mutex
    }
}

double Counter::value() const {
    return m_value;  // Retourner la valeur du compteur
}

bool Counter::isProtected() const {
    return p_mutex != nullptr;  // Vérifier si la protection par mutex est activée
}

volatile double& Counter::getValueRef() {
    return m_value;  // Retourner la référence à la valeur du compteur
}

