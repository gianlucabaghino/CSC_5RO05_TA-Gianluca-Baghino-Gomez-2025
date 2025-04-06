#ifndef CPULOOP_TD3_H
#define CPULOOP_TD3_H

#include "CalibratorTD3.h"
#include "Chrono.h"
#include "TimespecUtils.h"
#include <utility>

// Classe permettant de simuler une charge CPU pendant un certain temps
class CpuLoop {
public:
    // Constructeur : on passe une référence à un objet Calibrator pour calibrer la charge CPU
    CpuLoop(Calibrator& calibrator);

    // Méthode qui exécute une boucle pendant une durée donnée (en millisecondes)
    // et retourne un pair (durée réelle, nombre de boucles)
    std::pair<double, double> runTime(double duration_ms);

    // Méthode pour exécuter une certaine charge (en nombre de "ticks")
    void execute(int ticks);

private:
    Calibrator& m_calibrator; // Référence vers l'objet Calibrator utilisé pour ajuster la charge CPU
};

#endif // CPULOOP_TD3_H
