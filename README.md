# CSC_5RO05_TA-Gianluca-Baghino-Gomez-2025
Exercices du Cours RO05 / td-1 td-2 td-3 td-4

# TD1

## td1_a) Gestion simplifiée du temps Posix

Pour le compiler, suivez ces étapes :

1. Naviguez vers le répertoire du projet :
   ```sh
   cd MyProject/src/

2. Compilez les fichiers sources :
   ```sh
   arm-linux-g++ -Wall -Wextra MainPosix.cpp TimespecUtils.cpp -o td1a

3. Transférez le fichier compilé vers le dispositif ARM :
   ```sh
   rsync -avz td1a root@192.168.50.43:

Cela transférera le fichier vers la machine cible avec l'adresse IP 192.168.50.43.

4. Connectez-vous au dispositif ARM via SSH :
   ```sh
   ssh root@192.168.50.43

5. Exécutez le programme compilé :
   ```sh
   ./td1a

Sortie :
   ```sh
   2500.5 ms en secondes : 2500.5 ms
   Temps actuel : 714456 secondes et 592905998 nanosecondes
   Temps négatif : -3 secondes et 499500000 nanosecondes
   Temps additionné : 6 secondes et 500000 nanosecondes
   Temps soustrait : 0 secondes et 999500000 nanosecondes
   Somme des temps : 6 secondes et 500000 nanosecondes
   Différence des temps : 0 secondes et 999500000 nanosecondes
   ```

## td1_b) Classe Chrono

Pour le compiler, suivez ces étapes :

1. Naviguez vers le répertoire du projet :
   ```sh
   cd MyProject/src/

2. Compilez les fichiers sources :
   ```sh
   arm-linux-g++ -Wall -Wextra MainChrono.cpp Chrono.cpp TimespecUtils.cpp -o td1b

3. Transférez le fichier compilé vers le dispositif ARM :
   ```sh
   rsync -avz td1b root@192.168.50.43:

Cela transférera le fichier vers la machine cible avec l'adresse IP 192.168.50.43.

4. Connectez-vous au dispositif ARM via SSH :
   ```sh
   ssh root@192.168.50.43

5. Exécutez le programme compilé :
   ```sh
   ./td1b

Sortie :
   ```sh
   Chrono démarré !
   Temps du tour (secondes): 3s et 504947ns
   Temps écoulé (secondes): 3s et 605416ns
   Temps du tour (ms): 3000.61 ms
   ```

# TD2

## td2_a) Timers avec callback

Pour le compiler, suivez ces étapes :

1. Naviguez vers le répertoire du projet :
   ```sh
   cd MyProject/src/

2. Compilez les fichiers sources :
   ```sh
   arm-linux-g++ -Wall -Wextra td2a.cpp -o td2a

3. Transférez le fichier compilé vers le dispositif ARM :
   ```sh
   rsync -avz td2a root@192.168.50.43:

Cela transférera le fichier vers la machine cible avec l'adresse IP 192.168.50.43.

4. Connectez-vous au dispositif ARM via SSH :
   ```sh
   ssh root@192.168.50.43

5. Exécutez le programme compilé :
   ```sh
   ./td2a

Sortie :
   ```sh
   Timer démarré ! Appuyez sur Ctrl+C pour quitter manuellement si nécessaire.
   Compteur: 1
   Compteur: 2
   Compteur: 3
   Compteur: 4
   Compteur: 5
   Compteur: 6
   Compteur: 7
   Compteur: 8
   Compteur: 9
   Compteur: 10
   Compteur: 11
   Compteur: 12
   Compteur: 13
   Compteur: 14
   Compteur: 15
   Le timer a été arrêté après 15 incrémentations !
   ```

## td2_b) Classe Timer

**Éléments publics (Interface) :**

- `Timer()` (constructeur) et `~Timer()` (destructeur) doivent être publics pour la gestion du cycle de vie des objets.
- `start()`, `start_ms()` et `stop()` sont publics car ils forment l'API principale pour le contrôle du chronomètre.

**Élément protégé (Point d'extension) :**

- `callback()` est protégé car c'est une méthode virtuelle pure qui doit être implémentée par les classes dérivées (polymorphisme), et ne doit pas être appelée directement par les utilisateurs.

**Éléments privés (Détails d'implémentation) :**

- `tid` (`timer_t`) est privé pour encapsuler le gestionnaire de chronomètre POSIX.
- `call_callback()` est privé car c'est une fonction statique utilitaire pour la gestion des signaux POSIX, un mécanisme interne qui ne doit pas être exposé.
  
**Pourquoi cette structure ?**

- Les méthodes publiques fournissent une interface simple (démarrer/arrêter).
- La méthode protégée `callback()` permet la personnalisation dans les classes dérivées.
- Les membres privés cachent la complexité POSIX de bas niveau, assurant sécurité et encapsulation.

Pour le compiler, suivez ces étapes :

1. Naviguez vers le répertoire du projet :
   ```sh
   cd MyProject/src/

2. Compilez les fichiers sources :
   ```sh
   arm-linux-g++ -Wall -Wextra td2b.cpp -o td2b

3. Transférez le fichier compilé vers le dispositif ARM :
   ```sh
   rsync -avz td2b root@192.168.50.43:

Cela transférera le fichier vers la machine cible avec l'adresse IP 192.168.50.43.

4. Connectez-vous au dispositif ARM via SSH :
   ```sh
   ssh root@192.168.50.43

5. Exécutez le programme compilé :
   ```sh
   ./td2b

Sortie :
   ```sh
   Compte à rebours démarré !
   Compte à rebours : 10
   Compte à rebours : 9
   Compte à rebours : 8
   Compte à rebours : 7
   Compte à rebours : 6
   Compte à rebours : 5
   Compte à rebours : 4
   Compte à rebours : 3
   Compte à rebours : 2
   Compte à rebours : 1
   Compte à rebours : 0
   Compte à rebours terminé !
   ```

## td2_c) Fonction simple consommant du CPU

**Const-Correctness**

`getSample()` doit être `const` car :

- Il ne fait que lire les données membres (`m_iLoop`).
- Il ne modifie pas l'état de l'objet.
- Il correspond aux meilleures pratiques C++ pour les méthodes getter.

**Qualification des Variables**

`m_iLoop` (double) :

- Aucune qualification spéciale nécessaire.
- Accédé/modifié uniquement dans `runLoop()`.
- Pas de exigences de sécurité des threads dans ce cas d'utilisation.

`m_doStop` (bool) :

Doit être `volatile` si :

- Utilisé dans un contexte multithread.
- Modifié via des signaux/interruptions.
- Nécessité d'empêcher les optimisations du compilateur.

Pas nécessaire pour un fonctionnement mono-thread.

## Exemple d'Implémentation

```cpp
class Looper {
public:
    double getSample() const { return m_iLoop; } // Getter `const` correct
    
private:
    double m_iLoop;         // Membre régulier
    volatile bool m_doStop; // Drapeau prenant en compte les threads
};
```

Pour le compiler, suivez ces étapes :

1. Naviguez vers le répertoire du projet :
   ```sh
   cd MyProject/src/

2. Compilez les fichiers sources :
   ```sh
   arm-linux-g++ -Wall -Wextra td2c.cpp Chrono.cpp TimespecUtils.cpp -o td2c

3. Transférez le fichier compilé vers le dispositif ARM :
   ```sh
   rsync -avz td2c root@192.168.50.43:

Cela transférera le fichier vers la machine cible avec l'adresse IP 192.168.50.43.

4. Connectez-vous au dispositif ARM via SSH :
   ```sh
   ssh root@192.168.50.43

5. Exécutez le programme compilé avec différents nombres de boucles :
   ```sh
   ./td2c 100
   ./td2c 1000
   ./td2c 10000
   ./td2c 100000
   ./td2c 1000000
   ./td2c 10000000
   ./td2c 100000000
   ./td2c 1000000000
   ```
   
Sortie :
   ```sh
   nLoops = 100
   Temps d'exécution : 0.007656 millisecondes
   
   nLoops = 1000
   Temps d'exécution : 0.060156 millisecondes
   
   nLoops = 10000
   Temps d'exécution : 0.585469 millisecondes
   
   nLoops = 100000
   Temps d'exécution : 5.915 millisecondes
   
   nLoops = 1000000
   Temps d'exécution : 58.5209 millisecondes
   
   nLoops = 10000000
   Temps d'exécution : 584.381 millisecondes
   
   nLoops = 100000000
   Temps d'exécution : 5843.03 millisecondes
   
   nLoops = 1000000000
   Temps d'exécution : 58428.2 millisecondes
   ```

![Courbe des valeurs des temps d’exécution en fonction des valeurs de nLoops](TempsExecution_vs_nLoops_TD2C.png)

## td2_d) Échantillonage du temps d’exécution d’une fonction

Pour le compiler, suivez ces étapes :

1. Naviguez vers le répertoire du projet :
   ```sh
   cd MyProject/src/

2. Compilez les fichiers sources :
   ```sh
   arm-linux-g++ -Wall -Wextra td2d.cpp Chrono.cpp TimespecUtils.cpp -o td2d -lrt -pthread -lm

3. Transférez le fichier compilé vers le dispositif ARM :
   ```sh
   rsync -avz td2d root@192.168.50.43:

Cela transférera le fichier vers la machine cible avec l'adresse IP 192.168.50.43.

4. Connectez-vous au dispositif ARM via SSH :
   ```sh
   ssh root@192.168.50.43

5. Exécutez le programme compilé :
   ```sh
   ./td2d

Sortie :
   ```sh
   Résultats de la calibration :
   a = 5.24957e+06, b = 0
   
   Temps demandé : 0.5s
   Boucles prédites : 2.62478e+06
   Durée réelle : 0.499956s
   --------------------------------
   Temps demandé : 0.7s
   Boucles prédites : 3.6747e+06
   Durée réelle : 0.700035s
   --------------------------------
   Temps demandé : 0.9s
   Boucles prédites : 4.72461e+06
   Durée réelle : 0.899683s
   --------------------------------
   Temps demandé : 1.1s
   Boucles prédites : 5.77452e+06
   Durée réelle : 1.09967s
   --------------------------------
   Temps demandé : 1.3s
   Boucles prédites : 6.82444e+06
   Durée réelle : 1.29961s
   --------------------------------
   Temps demandé : 1.5s
   Boucles prédites : 7.87435e+06
   Durée réelle : 1.49943s
   --------------------------------
   Temps demandé : 1.7s
   Boucles prédites : 8.92426e+06
   Durée réelle : 1.6995s
   --------------------------------
   Temps demandé : 1.9s
   Boucles prédites : 9.97418e+06
   Durée réelle : 1.89936s
   --------------------------------
   Temps demandé : 2.1s
   Boucles prédites : 1.10241e+07
   Durée réelle : 2.09928s
   --------------------------------
   Temps demandé : 2.3s
   Boucles prédites : 1.2074e+07
   Durée réelle : 2.29915s
   --------------------------------
   Temps demandé : 2.5s
   Boucles prédites : 1.31239e+07
   Durée réelle : 2.49905s
   --------------------------------
   Temps demandé : 2.7s
   Boucles prédites : 1.41738e+07
   Durée réelle : 2.69912s
   --------------------------------
   Temps demandé : 2.9s
   Boucles prédites : 1.52237e+07
   Durée réelle : 2.89889s
   --------------------------------
   Temps demandé : 3.1s
   Boucles prédites : 1.62737e+07
   Durée réelle : 3.09886s
   --------------------------------
   Temps demandé : 3.3s
   Boucles prédites : 1.73236e+07
   Durée réelle : 3.29881s
   --------------------------------
   Temps demandé : 3.5s
   Boucles prédites : 1.83735e+07
   Durée réelle : 3.4987s
   --------------------------------
   Temps demandé : 3.7s
   Boucles prédites : 1.94234e+07
   Durée réelle : 3.69877s
   --------------------------------
   Temps demandé : 3.9s
   Boucles prédites : 2.04733e+07
   Durée réelle : 3.89891s
   --------------------------------
   Temps demandé : 4.1s
   Boucles prédites : 2.15232e+07
   Durée réelle : 4.09848s
   --------------------------------
   Temps demandé : 4.3s
   Boucles prédites : 2.25731e+07
   Durée réelle : 4.29847s
   --------------------------------
   Temps demandé : 4.5s
   Boucles prédites : 2.3623e+07
   Durée réelle : 4.49829s
   --------------------------------
   Temps demandé : 4.7s
   Boucles prédites : 2.4673e+07
   Durée réelle : 4.69835s
   --------------------------------
   Temps demandé : 4.9s
   Boucles prédites : 2.57229e+07
   Durée réelle : 4.89814s
   --------------------------------
   Temps demandé : 5.1s
   Boucles prédites : 2.67728e+07
   Durée réelle : 5.09803s
   --------------------------------
   Temps demandé : 5.3s
   Boucles prédites : 2.78227e+07
   Durée réelle : 5.29801s
   --------------------------------
   Temps demandé : 5.5s
   Boucles prédites : 2.88726e+07
   Durée réelle : 5.49805s
   --------------------------------
   Temps demandé : 5.7s
   Boucles prédites : 2.99225e+07
   Durée réelle : 5.69789s
   --------------------------------
   Temps demandé : 5.9s
   Boucles prédites : 3.09724e+07
   Durée réelle : 5.8978s
   --------------------------------
   Temps demandé : 6.1s
   Boucles prédites : 3.20224e+07
   Durée réelle : 6.09765s
   --------------------------------
   Temps demandé : 6.3s
   Boucles prédites : 3.30723e+07
   Durée réelle : 6.29778s
   --------------------------------
   Temps demandé : 6.5s
   Boucles prédites : 3.41222e+07
   Durée réelle : 6.49772s
   --------------------------------
   Temps demandé : 6.7s
   Boucles prédites : 3.51721e+07
   Durée réelle : 6.69746s
   --------------------------------
   Temps demandé : 6.9s
   Boucles prédites : 3.6222e+07
   Durée réelle : 6.89743s
   --------------------------------
   Temps demandé : 7.1s
   Boucles prédites : 3.72719e+07
   Durée réelle : 7.09742s
   --------------------------------
   Temps demandé : 7.3s
   Boucles prédites : 3.83218e+07
   Durée réelle : 7.29715s
   --------------------------------
   Temps demandé : 7.5s
   Boucles prédites : 3.93717e+07
   Durée réelle : 7.49721s
   --------------------------------
   Temps demandé : 7.7s
   Boucles prédites : 4.04217e+07
   Durée réelle : 7.69706s
   --------------------------------
   Temps demandé : 7.9s
   Boucles prédites : 4.14716e+07
   Durée réelle : 7.897s
   --------------------------------
   Temps demandé : 8.1s
   Boucles prédites : 4.25215e+07
   Durée réelle : 8.09716s
   --------------------------------
   Temps demandé : 8.3s
   Boucles prédites : 4.35714e+07
   Durée réelle : 8.297s
   --------------------------------
   Temps demandé : 8.5s
   Boucles prédites : 4.46213e+07
   Durée réelle : 8.49681s
   --------------------------------
   Temps demandé : 8.7s
   Boucles prédites : 4.56712e+07
   Durée réelle : 8.69672s
   --------------------------------
   Temps demandé : 8.9s
   Boucles prédites : 4.67211e+07
   Durée réelle : 8.89721s
   --------------------------------
   Temps demandé : 9.1s
   Boucles prédites : 4.77711e+07
   Durée réelle : 9.09677s
   --------------------------------
   Temps demandé : 9.3s
   Boucles prédites : 4.8821e+07
   Durée réelle : 9.29649s
   --------------------------------
   Temps demandé : 9.5s
   Boucles prédites : 4.98709e+07
   Durée réelle : 9.49658s
   --------------------------------
   Temps demandé : 9.7s
   Boucles prédites : 5.09208e+07
   Durée réelle : 9.69642s
   --------------------------------
   Temps demandé : 9.9s
   Boucles prédites : 5.19707e+07
   Durée réelle : 9.89627s
   --------------------------------
   ```

## td2_e) Classe consommatrice de CPU durant un temps donné

Pour le compiler, suivez ces étapes :

1. Naviguez vers le répertoire du projet :
   ```sh
   cd MyProject/src/

2. Compilez les fichiers sources :
   ```sh
   arm-linux-g++ -Wall -Wextra td2e.cpp Chrono.cpp TimespecUtils.cpp -o td2e -lrt -pthread -lm

3. Transférez le fichier compilé vers le dispositif ARM :
   ```sh
   rsync -avz td2e root@192.168.50.43:

Cela transférera le fichier vers la machine cible avec l'adresse IP 192.168.50.43.

4. Connectez-vous au dispositif ARM via SSH :
   ```sh
   ssh root@192.168.50.43

5. Exécutez le programme compilé :
   ```sh
   ./td2e

Sortie :
   ```sh
   
   ```

# TD3

## td3_a_1)

1. Naviguez vers le répertoire du projet :
   ```sh
   cd MyProject/src/

2. Compilez les fichiers sources :
   ```sh
   arm-linux-g++ -Wall -Wextra td2e.cpp Chrono.cpp TimespecUtils.cpp -o td2e -lrt -pthread -lm

3. Transférez le fichier compilé vers le dispositif ARM :
   ```sh
   rsync -avz td2e root@192.168.50.43:

Cela transférera le fichier vers la machine cible avec l'adresse IP 192.168.50.43.

4. Connectez-vous au dispositif ARM via SSH :
   ```sh
   ssh root@192.168.50.43

5. Exécutez le programme compilé :
   ```sh
   ./td2e

Sortie :
   ```sh
   
   ```

## td3_a_2)

1. Naviguez vers le répertoire du projet :
   ```sh
   cd MyProject/src/

2. Compilez les fichiers sources :
   ```sh
   arm-linux-g++ -Wall -Wextra td2e.cpp Chrono.cpp TimespecUtils.cpp -o td2e -lrt -pthread -lm

3. Transférez le fichier compilé vers le dispositif ARM :
   ```sh
   rsync -avz td2e root@192.168.50.43:

Cela transférera le fichier vers la machine cible avec l'adresse IP 192.168.50.43.

4. Connectez-vous au dispositif ARM via SSH :
   ```sh
   ssh root@192.168.50.43

5. Exécutez le programme compilé :
   ```sh
   ./td2e

Sortie :
   ```sh
   
   ```

## td3_a_3)

1. Naviguez vers le répertoire du projet :
   ```sh
   cd MyProject/src/

2. Compilez les fichiers sources :
   ```sh
   arm-linux-g++ -Wall -Wextra td2e.cpp Chrono.cpp TimespecUtils.cpp -o td2e -lrt -pthread -lm

3. Transférez le fichier compilé vers le dispositif ARM :
   ```sh
   rsync -avz td2e root@192.168.50.43:

Cela transférera le fichier vers la machine cible avec l'adresse IP 192.168.50.43.

4. Connectez-vous au dispositif ARM via SSH :
   ```sh
   ssh root@192.168.50.43

5. Exécutez le programme compilé :
   ```sh
   ./td2e

Sortie :
   ```sh
   
   ```

## td3_b)

1. Naviguez vers le répertoire du projet :
   ```sh
   cd MyProject/src/

2. Compilez les fichiers sources :
   ```sh
   arm-linux-g++ -Wall -Wextra td2e.cpp Chrono.cpp TimespecUtils.cpp -o td2e -lrt -pthread -lm

3. Transférez le fichier compilé vers le dispositif ARM :
   ```sh
   rsync -avz td2e root@192.168.50.43:

Cela transférera le fichier vers la machine cible avec l'adresse IP 192.168.50.43.

4. Connectez-vous au dispositif ARM via SSH :
   ```sh
   ssh root@192.168.50.43

5. Exécutez le programme compilé :
   ```sh
   ./td2e

Sortie :
   ```sh
   
   ```

## td3_c)

1. Naviguez vers le répertoire du projet :
   ```sh
   cd MyProject/src/

2. Compilez les fichiers sources :
   ```sh
   arm-linux-g++ -Wall -Wextra td2e.cpp Chrono.cpp TimespecUtils.cpp -o td2e -lrt -pthread -lm

3. Transférez le fichier compilé vers le dispositif ARM :
   ```sh
   rsync -avz td2e root@192.168.50.43:

Cela transférera le fichier vers la machine cible avec l'adresse IP 192.168.50.43.

4. Connectez-vous au dispositif ARM via SSH :
   ```sh
   ssh root@192.168.50.43

5. Exécutez le programme compilé :
   ```sh
   ./td2e

Sortie :
   ```sh
   
   ```

## td3_d)

1. Naviguez vers le répertoire du projet :
   ```sh
   cd MyProject/src/

2. Compilez les fichiers sources :
   ```sh
   arm-linux-g++ -Wall -Wextra td2e.cpp Chrono.cpp TimespecUtils.cpp -o td2e -lrt -pthread -lm

3. Transférez le fichier compilé vers le dispositif ARM :
   ```sh
   rsync -avz td2e root@192.168.50.43:

Cela transférera le fichier vers la machine cible avec l'adresse IP 192.168.50.43.

4. Connectez-vous au dispositif ARM via SSH :
   ```sh
   ssh root@192.168.50.43

5. Exécutez le programme compilé :
   ```sh
   ./td2e

Sortie :
   ```sh
   
   ```

## td3_e)

1. Naviguez vers le répertoire du projet :
   ```sh
   cd MyProject/src/

2. Compilez les fichiers sources :
   ```sh
   arm-linux-g++ -Wall -Wextra td2e.cpp Chrono.cpp TimespecUtils.cpp -o td2e -lrt -pthread -lm

3. Transférez le fichier compilé vers le dispositif ARM :
   ```sh
   rsync -avz td2e root@192.168.50.43:

Cela transférera le fichier vers la machine cible avec l'adresse IP 192.168.50.43.

4. Connectez-vous au dispositif ARM via SSH :
   ```sh
   ssh root@192.168.50.43

5. Exécutez le programme compilé :
   ```sh
   ./td2e

Sortie :
   ```sh
   
   ```

# TD4

## td4_a)

1. Naviguez vers le répertoire du projet :
   ```sh
   cd MyProject/src/

2. Compilez les fichiers sources :
   ```sh
   arm-linux-g++ -Wall -Wextra td2e.cpp Chrono.cpp TimespecUtils.cpp -o td2e -lrt -pthread -lm

3. Transférez le fichier compilé vers le dispositif ARM :
   ```sh
   rsync -avz td2e root@192.168.50.43:

Cela transférera le fichier vers la machine cible avec l'adresse IP 192.168.50.43.

4. Connectez-vous au dispositif ARM via SSH :
   ```sh
   ssh root@192.168.50.43

5. Exécutez le programme compilé :
   ```sh
   ./td2e

Sortie :
   ```sh
   
   ```

## td4_b)

1. Naviguez vers le répertoire du projet :
   ```sh
   cd MyProject/src/

2. Compilez les fichiers sources :
   ```sh
   arm-linux-g++ -Wall -Wextra td2e.cpp Chrono.cpp TimespecUtils.cpp -o td2e -lrt -pthread -lm

3. Transférez le fichier compilé vers le dispositif ARM :
   ```sh
   rsync -avz td2e root@192.168.50.43:

Cela transférera le fichier vers la machine cible avec l'adresse IP 192.168.50.43.

4. Connectez-vous au dispositif ARM via SSH :
   ```sh
   ssh root@192.168.50.43

5. Exécutez le programme compilé :
   ```sh
   ./td2e

Sortie :
   ```sh
   
   ```
