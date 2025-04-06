#include "Chrono.h"
#include <iostream>

Chrono::Chrono() : m_isActive(false) {
    restart();  // Initialiser le chrono lorsqu'un objet est créé
}

void Chrono::restart() {
    m_startTime = timespec_now();  // Définir l'heure actuelle comme heure de départ
    m_stopTime = m_startTime;      // L'heure d'arrêt est égale à l'heure de départ
    m_isActive = true;             // Activer le chrono
}

timespec Chrono::stop() {
    if (m_isActive) {
        m_stopTime = timespec_now();  // L'heure d'arrêt est l'heure actuelle
        m_isActive = false;           // Désactiver le chrono
        return timespec_subtract(m_stopTime, m_startTime); // Retourner le temps écoulé
    }
    return timespec{0, 0};  // Retourner 0 si stop est appelé avant restart
}

timespec Chrono::lap() {
    if (m_isActive) {
        return timespec_subtract(timespec_now(), m_startTime); // Si actif, retourner le temps depuis restart
    } else {
        return timespec_subtract(m_stopTime, m_startTime); // Si arrêté, retourner le temps entre restart et stop
    }
}

double Chrono::lap_ms() {
    timespec lapTime = lap(); // Obtenir le temps de tour en tant que timespec
    return timespec_to_ms(lapTime); // Convertir en millisecondes et retourner
}

