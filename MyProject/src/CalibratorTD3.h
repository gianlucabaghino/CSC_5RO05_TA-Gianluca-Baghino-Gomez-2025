#ifndef CALIBRATOR_TD3_H
#define CALIBRATOR_TD3_H

#include "TimerTD3.h"
#include "LooperTD3.h"
#include <vector>

// Classe Calibrator : hérite de la classe Timer
// Elle permet de calibrer le nombre de boucles nécessaires pour simuler une certaine durée
class Calibrator : public Timer {
public:
    // Constructeur : prend une période d’échantillonnage en millisecondes et un nombre d’échantillons
    Calibrator(double samplingPeriod_ms, unsigned int nSamples);

    // Méthode appelée périodiquement (override de Timer)
    void callback() override;

    // Renvoie le nombre de boucles à faire pour simuler une durée t (en millisecondes)
    double nLoops(double t) const;

    // Accesseurs aux coefficients de la régression linéaire (a et b)
    double getA() const;
    double getB() const;

    // Attend un certain temps (delay donné en timespec)
    void wait(const timespec& delay_ts);

private:
    // Calcule les coefficients a et b de la régression linéaire
    void computeRegression();

    double m_samplingPeriod_ms;     // Période d’échantillonnage en millisecondes
    unsigned int m_nSamples;        // Nombre total d’échantillons
    double m_a = 0, m_b = 0;        // Coefficients de régression linéaire (a : pente, b : ordonnée à l’origine)
    Looper m_looper;                // Objet Looper pour simuler une charge CPU (boucles)
    std::vector<double> m_samples;  // Vecteur pour stocker les durées mesurées (en nombre de boucles)
};

#endif // CALIBRATOR_TD3_H


