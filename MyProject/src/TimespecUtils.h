#ifndef TIMESPEC_UTILS_H_INCLUDED
#define TIMESPEC_UTILS_H_INCLUDED

#include <time.h>

// Convertit un temps 'timespec' en millisecondes
double timespec_to_ms(const timespec& time_ts);

// Convertit un nombre de millisecondes en 'timespec'
timespec timespec_from_ms(double time_ms);

// Retourne le temps actuel sous forme de 'timespec'
timespec timespec_now();

// Inverse un temps ('timespec')
timespec timespec_negate(const timespec& time_ts);

// Additionne deux temps 'timespec'
timespec timespec_add(const timespec& time1_ts, const timespec& time2_ts);

// Soustrait un temps 'timespec' d'un autre
timespec timespec_subtract(const timespec& time1_ts, const timespec& time2_ts);

// Ajoute un délai en millisecondes à un temps 'timespec'
timespec timespec_add_ms(const timespec &time, double ms);

// Fait une pause pour le temps spécifié (en 'timespec')
void timespec_wait(const timespec& delay_ts);

// Opérateurs pour manipuler les 'timespec' comme des scalaires
timespec operator- (const timespec& time_ts); // Retourne le négatif de 'time_ts'
timespec operator+ (const timespec& time1_ts, const timespec& time2_ts); // Additionne les deux temps
timespec operator- (const timespec& time1_ts, const timespec& time2_ts); // Soustrait 'time2_ts' de 'time1_ts'
timespec& operator+= (timespec& time_ts, const timespec& delay_ts); // Ajoute 'delay_ts' à 'time_ts'
timespec& operator-= (timespec& time_ts, const timespec& delay_ts); // Soustrait 'delay_ts' de 'time_ts'
bool operator== (const timespec& time1_ts, const timespec& time2_ts); // Compare les deux temps pour l'égalité
bool operator!= (const timespec& time1_ts, const timespec& time2_ts); // Compare les deux temps pour l'inégalité
bool operator< (const timespec& time1_ts, const timespec& time2_ts); // Vérifie si 'time1_ts' est plus petit que 'time2_ts'
bool operator> (const timespec& time1_ts, const timespec& time2_ts); // Vérifie si 'time1_ts' est plus grand que 'time2_ts'

#endif // TIMESPEC_UTILS_H_INCLUDED

