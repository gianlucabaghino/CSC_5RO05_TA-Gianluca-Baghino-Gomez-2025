#include "TimespecUtils.h"
#include <iostream>
#include <ctime>
#include <cmath>
#include <unistd.h> // Pour nanosleep

// Convertit un temps timespec en millisecondes
double timespec_to_ms(const timespec& time_ts)
{
    return time_ts.tv_sec * 1000.0 + time_ts.tv_nsec / 1000000.0; // Convertit les secondes et nanosecondes en millisecondes
}

// Convertit un temps en millisecondes en timespec
timespec timespec_from_ms(double time_ms)
{
    timespec result;
    result.tv_sec = static_cast<time_t>(time_ms / 1000); // Calcul de la seconde à partir des millisecondes
    result.tv_nsec = static_cast<long>((time_ms - result.tv_sec * 1000) * 1000000); // Reste des millisecondes converti en nanosecondes
    return result;
}

// Retourne le temps actuel
timespec timespec_now()
{
    timespec current_time;
    clock_gettime(CLOCK_REALTIME, &current_time); // Récupère l'heure système (temps réel)
    return current_time;
}

// Renverse un temps (opposé)
timespec timespec_negate(const timespec& time_ts)
{
    timespec result;
    result.tv_sec = -time_ts.tv_sec; // Inverse les secondes
    result.tv_nsec = (time_ts.tv_nsec == 0) ? 0 : 1000000000 - time_ts.tv_nsec; // Inverse les nanosecondes
    if (time_ts.tv_nsec != 0) {
        result.tv_sec -= 1; // Si les nanosecondes sont inversées, ajuster les secondes
    }
    return result;
}

// Additionne deux temps timespec
timespec timespec_add(const timespec& time1_ts, const timespec& time2_ts)
{
    timespec result;
    result.tv_sec = time1_ts.tv_sec + time2_ts.tv_sec; // Addition des secondes
    result.tv_nsec = time1_ts.tv_nsec + time2_ts.tv_nsec; // Addition des nanosecondes

    if (result.tv_nsec >= 1000000000) {
        result.tv_sec += 1; // Si les nanosecondes sont supérieures à 1 seconde, ajuster
        result.tv_nsec -= 1000000000; // Réduire les nanosecondes à moins de 1 seconde
    }

    return result;
}

// Soustrait deux temps timespec
timespec timespec_subtract(const timespec& time1_ts, const timespec& time2_ts)
{
    timespec result;
    result.tv_sec = time1_ts.tv_sec - time2_ts.tv_sec; // Soustraction des secondes
    result.tv_nsec = time1_ts.tv_nsec - time2_ts.tv_nsec; // Soustraction des nanosecondes

    if (result.tv_nsec < 0) {
        result.tv_sec -= 1; // Si les nanosecondes sont négatives, ajuster les secondes
        result.tv_nsec += 1000000000; // Ajouter une seconde aux nanosecondes
    }

    return result;
}

// Met la tâche en pause pour le temps spécifié
void timespec_wait(const timespec& delay_ts)
{
    timespec remaining_time = delay_ts;
    while (nanosleep(&remaining_time, &remaining_time) == -1 && errno == EINTR);
}

// Opérateurs pour manipuler les timespec comme des scalaires

// Négatif d'un temps timespec
timespec operator- (const timespec& time_ts)
{
    timespec result;
    result.tv_sec = -time_ts.tv_sec; // Inverse les secondes
    result.tv_nsec = -time_ts.tv_nsec; // Inverse les nanosecondes
    if (result.tv_nsec < 0) {
        result.tv_sec -= 1; // Si les nanosecondes sont négatives, ajuster
        result.tv_nsec += 1000000000; // Ajouter une seconde aux nanosecondes
    }
    return result;
}

// Additionne deux temps timespec avec l'opérateur "+"
timespec operator+ (const timespec& time1_ts, const timespec& time2_ts)
{
    return timespec_add(time1_ts, time2_ts); // Utilise la fonction d'addition
}

// Soustrait deux temps timespec avec l'opérateur "-"
timespec operator- (const timespec& time1_ts, const timespec& time2_ts)
{
    return timespec_subtract(time1_ts, time2_ts); // Utilise la fonction de soustraction
}

// Modifie un temps timespec en y ajoutant un autre temps
timespec& operator+= (timespec& time_ts, const timespec& delay_ts)
{
    time_ts = timespec_add(time_ts, delay_ts); // Additionne et met à jour le temps
    return time_ts;
}

// Modifie un temps timespec en y soustrayant un autre temps
timespec& operator-= (timespec& time_ts, const timespec& delay_ts)
{
    time_ts = timespec_subtract(time_ts, delay_ts); // Soustrait et met à jour le temps
    return time_ts;
}

// Additionne un nombre de millisecondes à un temps timespec
timespec timespec_add_ms(const timespec &time, double ms) {
    timespec result = time;
    result.tv_nsec += static_cast<long>(ms * 1e6);  // Convertir les millisecondes en nanosecondes
    while (result.tv_nsec >= 1e9) {
        result.tv_nsec -= 1e9; // Si les nanosecondes dépassent une seconde, ajuster
        result.tv_sec += 1;    // Ajouter 1 seconde
    }
    return result;
}

// Vérifie si deux temps timespec sont égaux
bool operator== (const timespec& time1_ts, const timespec& time2_ts)
{
    return time1_ts.tv_sec == time2_ts.tv_sec && time1_ts.tv_nsec == time2_ts.tv_nsec; // Comparaison des secondes et des nanosecondes
}

// Vérifie si deux temps timespec sont différents
bool operator!= (const timespec& time1_ts, const timespec& time2_ts)
{
    return !(time1_ts == time2_ts); // Utilise l'opérateur "==" pour comparer
}

// Vérifie si un temps timespec est inférieur à un autre
bool operator< (const timespec& time1_ts, const timespec& time2_ts)
{
    if (time1_ts.tv_sec < time2_ts.tv_sec)
        return true;
    else if (time1_ts.tv_sec == time2_ts.tv_sec && time1_ts.tv_nsec < time2_ts.tv_nsec)
        return true;
    return false;
}

// Vérifie si un temps timespec est supérieur à un autre
bool operator> (const timespec& time1_ts, const timespec& time2_ts)
{
    return time2_ts < time1_ts; // Utilise l'opérateur "<" pour comparer
}


