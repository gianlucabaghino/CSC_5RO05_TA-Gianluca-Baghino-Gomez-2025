#include "CalibratorTD3.h"
#include "TimespecUtils.h"
#include <cfloat>

// Constructeur de la classe Calibrator
// Il initialise la période d’échantillonnage et le nombre d’échantillons
// Lance une boucle d'exécution pour calibrer les boucles CPU
Calibrator::Calibrator(double samplingPeriod_ms, unsigned int nSamples)
    : m_samplingPeriod_ms(samplingPeriod_ms), m_nSamples(nSamples) {

    // Convertit la période d’échantillonnage en timespec
    timespec ts = Timer::timespec_from_ms(m_samplingPeriod_ms);
    // Démarre le timer avec cette période
    start(ts, true);
    // Exécute la boucle avec un très grand nombre de tours (DBL_MAX)
    m_looper.runLoop(DBL_MAX);
    // Arrête le timer
    stop();
    // Calcule les coefficients de régression après les échantillons
    computeRegression();
}

// Fonction appelée à chaque période d’échantillonnage
// Elle enregistre les différences entre les échantillons successifs
void Calibrator::callback() {
    static double lastSample = 0;
    double current = m_looper.getSample();
    // Enregistre la différence entre l’échantillon actuel et le précédent
    m_samples.push_back(current - lastSample);
    lastSample = current;

    // Si le nombre d’échantillons atteint la limite, on arrête la boucle
    if (m_samples.size() >= m_nSamples) {
        m_looper.stopLoop();
    }
}

// Calcule les coefficients de régression linéaire
// pour estimer le nombre de boucles CPU par seconde
void Calibrator::computeRegression() {
    double sumY = 0;
    const double interval_s = m_samplingPeriod_ms / 1000.0;

    // Somme de tous les échantillons enregistrés
    for (unsigned i = 0; i < m_samples.size(); ++i) {
        sumY += m_samples[i];
    }

    // Calcul du coefficient a (pente de la régression)
    m_a = sumY / (m_samples.size() * interval_s);
    // Le coefficient b est nul (régression sans ordonnée à l’origine)
    m_b = 0;
}

// Retourne le nombre estimé de boucles CPU pour un temps donné (en secondes)
double Calibrator::nLoops(double t) const {
    return m_a * t;
}

// Getter pour le coefficient a
double Calibrator::getA() const {
    return m_a;
}

// Getter pour le coefficient b
double Calibrator::getB() const {
    return m_b;
}

// Attend pendant un délai donné (timespec)
void Calibrator::wait(const timespec& delay_ts) {
    // Appelle la fonction d’attente définie dans TimespecUtils
    timespec_wait(delay_ts);
}
