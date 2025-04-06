#include <iostream>
#include "Chrono.h"  // Inclusion de la classe Chrono du TD1

// Classe Looper qui exécute une boucle de calcul intensif
class Looper
{
public:
    // Constructeur : initialise les attributs
    Looper() : m_iLoop(0.0), m_doStop(false) {}

    // Exécute une boucle de calcul jusqu'à nLoops ou jusqu'à un arrêt forcé
    double runLoop(int nLoops)
    {
        m_iLoop = 0.0;     // Réinitialisation du compteur
        m_doStop = false;  // Réinitialisation de l'indicateur d'arrêt

        while (m_iLoop < nLoops && !m_doStop)
        {
            m_iLoop += 1.0; // Incrémentation
        }

        return m_iLoop; // Retour de la valeur finale
    }

    // Retourne la valeur actuelle du compteur (const car ne modifie pas l'objet)
    double getSample() const
    {
        return m_iLoop;
    }

    // Permet d'arrêter la boucle en modifiant l'indicateur d'arrêt
    void stopLoop()
    {
        m_doStop = true;
    }

private:
    double m_iLoop;    // Compteur principal
    bool m_doStop;     // Indicateur d'arrêt
};

// Fonction principale du programme
int main(int argc, char* argv[])
{
    // Vérification du nombre d'arguments
    if (argc != 2)
    {
        std::cerr << "Erreur : veuillez fournir le nombre de boucles en argument." << std::endl;
        return 1;
    }

    // Conversion de l'argument en entier
    int nLoops = std::stoi(argv[1]);

    // Création de l'objet Looper
    Looper looper;

    // Mesure du temps d'exécution avec Chrono
    Chrono chrono;
    chrono.restart(); // Démarrage du chrono

    // Exécution de la boucle
    looper.runLoop(nLoops);

    // Temps écoulé
    double elapsedTime = chrono.lap_ms();

    // Affichage des résultats
    std::cout << "nLoops = " << nLoops << std::endl;
    std::cout << "Temps d'exécution : " << elapsedTime << " millisecondes" << std::endl;

    return 0;
}
