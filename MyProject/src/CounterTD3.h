#ifndef COUNTERTD3_H
#define COUNTERTD3_H

#include <memory>
#include <pthread.h>

// Classe Counter pour gérer un compteur avec protection par mutex
class Counter {
private:
    volatile double m_value;  // La valeur du compteur
    std::unique_ptr<pthread_mutex_t> p_mutex;  // Mutex pour protéger la valeur du compteur

public:
    // Constructeur de la classe Counter, prend un booléen pour déterminer si le compteur doit être protégé
    explicit Counter(bool protect);  

    // Destructeur de la classe Counter
    ~Counter();  

    // Méthode pour incrémenter la valeur du compteur
    void increment();  

    // Méthode pour obtenir la valeur du compteur
    double value() const;  

    // Méthode pour vérifier si le compteur est protégé par un mutex
    bool isProtected() const;  

    // Méthode pour obtenir une référence à la valeur du compteur
    volatile double& getValueRef();  
};

#endif // COUNTERTD3_H




