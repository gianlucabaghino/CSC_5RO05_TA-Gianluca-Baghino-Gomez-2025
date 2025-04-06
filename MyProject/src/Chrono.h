#ifndef CHRONO_H_INCLUDED
#define CHRONO_H_INCLUDED

#include "TimespecUtils.h"

// Classe Chrono : Permet de mesurer et suivre le temps écoulé
class Chrono {
public:
    // Constructeur : Initialise le chrono à l'heure actuelle
    Chrono();                      

    // Redémarre le chrono en définissant l'heure de départ et d'arrêt à l'heure actuelle
    void restart();                

    // Arrête le chrono et retourne le temps écoulé sous forme de timespec
    timespec stop();               

    // Renvoie le temps écoulé depuis le dernier redémarrage ou arrêt sous forme de timespec
    timespec lap();                 

    // Renvoie le temps écoulé en millisecondes depuis le dernier redémarrage ou arrêt
    double lap_ms();                

private:
    // Heure de début du chrono
    timespec m_startTime;          

    // Heure d'arrêt du chrono
    timespec m_stopTime;           

    // Indicateur si le chrono est actif (démarré mais pas encore arrêté)
    bool m_isActive;                
};

#endif // CHRONO_H_INCLUDED


