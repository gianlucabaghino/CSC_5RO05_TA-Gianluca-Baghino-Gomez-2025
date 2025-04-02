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
   Temps actuel : 534058 secondes et 333814554 nanosecondes
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
   Temps du tour (secondes): 3s et 442863ns
   Temps écoulé (secondes): 3s et 536613ns
   Temps du tour (ms): 3000.54 ms
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
   arm-linux-g++ -Wall -Wextra -std=c++17 td2d.cpp Chrono.cpp TimespecUtils.cpp -o td2d -lrt -pthread -lm

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
   a = 5249.08, b = 1.04901e+06
   
   Temps demandé : 0.5s
   Boucles prédites : 1.05164e+06
   Durée réelle : 0.200365s
   --------------------------------
   Temps demandé : 0.7s
   Boucles prédites : 1.05269e+06
   Durée réelle : 0.200426s
   --------------------------------
   Temps demandé : 0.9s
   Boucles prédites : 1.05374e+06
   Durée réelle : 0.20102s
   --------------------------------
   Temps demandé : 1.1s
   Boucles prédites : 1.05479e+06
   Durée réelle : 0.201076s
   --------------------------------
   Temps demandé : 1.3s
   Boucles prédites : 1.05584e+06
   Durée réelle : 0.201483s
   --------------------------------
   Temps demandé : 1.5s
   Boucles prédites : 1.05689e+06
   Durée réelle : 0.201553s
   --------------------------------
   Temps demandé : 1.7s
   Boucles prédites : 1.05794e+06
   Durée réelle : 0.201808s
   --------------------------------
   Temps demandé : 1.9s
   Boucles prédites : 1.05899e+06
   Durée réelle : 0.202027s
   --------------------------------
   Temps demandé : 2.1s
   Boucles prédites : 1.06003e+06
   Durée réelle : 0.201874s
   --------------------------------
   Temps demandé : 2.3s
   Boucles prédites : 1.06108e+06
   Durée réelle : 0.202452s
   --------------------------------
   Temps demandé : 2.5s
   Boucles prédites : 1.06213e+06
   Durée réelle : 0.202324s
   --------------------------------
   Temps demandé : 2.7s
   Boucles prédites : 1.06318e+06
   Durée réelle : 0.202468s
   --------------------------------
   Temps demandé : 2.9s
   Boucles prédites : 1.06423e+06
   Durée réelle : 0.20274s
   --------------------------------
   Temps demandé : 3.1s
   Boucles prédites : 1.06528e+06
   Durée réelle : 0.202868s
   --------------------------------
   Temps demandé : 3.3s
   Boucles prédites : 1.06633e+06
   Durée réelle : 0.203235s
   --------------------------------
   Temps demandé : 3.5s
   Boucles prédites : 1.06738e+06
   Durée réelle : 0.203336s
   --------------------------------
   Temps demandé : 3.7s
   Boucles prédites : 1.06843e+06
   Durée réelle : 0.203474s
   --------------------------------
   Temps demandé : 3.9s
   Boucles prédites : 1.06948e+06
   Durée réelle : 0.203733s
   --------------------------------
   Temps demandé : 4.1s
   Boucles prédites : 1.07053e+06
   Durée réelle : 0.203899s
   --------------------------------
   Temps demandé : 4.3s
   Boucles prédites : 1.07158e+06
   Durée réelle : 0.204249s
   --------------------------------
   Temps demandé : 4.5s
   Boucles prédites : 1.07263e+06
   Durée réelle : 0.204331s
   --------------------------------
   Temps demandé : 4.7s
   Boucles prédites : 1.07368e+06
   Durée réelle : 0.204446s
   --------------------------------
   Temps demandé : 4.9s
   Boucles prédites : 1.07473e+06
   Durée réelle : 0.204725s
   --------------------------------
   Temps demandé : 5.1s
   Boucles prédites : 1.07578e+06
   Durée réelle : 0.204862s
   --------------------------------
   Temps demandé : 5.3s
   Boucles prédites : 1.07683e+06
   Durée réelle : 0.205217s
   --------------------------------
   Temps demandé : 5.5s
   Boucles prédites : 1.07788e+06
   Durée réelle : 0.205279s
   --------------------------------
   Temps demandé : 5.7s
   Boucles prédites : 1.07893e+06
   Durée réelle : 0.205482s
   --------------------------------
   Temps demandé : 5.9s
   Boucles prédites : 1.07998e+06
   Durée réelle : 0.205693s
   --------------------------------
   Temps demandé : 6.1s
   Boucles prédites : 1.08103e+06
   Durée réelle : 0.205867s
   --------------------------------
   Temps demandé : 6.3s
   Boucles prédites : 1.08208e+06
   Durée réelle : 0.20612s
   --------------------------------
   Temps demandé : 6.5s
   Boucles prédites : 1.08313e+06
   Durée réelle : 0.206409s
   --------------------------------
   Temps demandé : 6.7s
   Boucles prédites : 1.08418e+06
   Durée réelle : 0.206461s
   --------------------------------
   Temps demandé : 6.9s
   Boucles prédites : 1.08523e+06
   Durée réelle : 0.206712s
   --------------------------------
   Temps demandé : 7.1s
   Boucles prédites : 1.08628e+06
   Durée réelle : 0.206855s
   --------------------------------
   Temps demandé : 7.3s
   Boucles prédites : 1.08733e+06
   Durée réelle : 0.207106s
   --------------------------------
   Temps demandé : 7.5s
   Boucles prédites : 1.08838e+06
   Durée réelle : 0.207412s
   --------------------------------
   Temps demandé : 7.7s
   Boucles prédites : 1.08943e+06
   Durée réelle : 0.207442s
   --------------------------------
   Temps demandé : 7.9s
   Boucles prédites : 1.09048e+06
   Durée réelle : 0.207701s
   --------------------------------
   Temps demandé : 8.1s
   Boucles prédites : 1.09153e+06
   Durée réelle : 0.207851s
   --------------------------------
   Temps demandé : 8.3s
   Boucles prédites : 1.09258e+06
   Durée réelle : 0.208115s
   --------------------------------
   Temps demandé : 8.5s
   Boucles prédites : 1.09363e+06
   Durée réelle : 0.208417s
   --------------------------------
   Temps demandé : 8.7s
   Boucles prédites : 1.09468e+06
   Durée réelle : 0.208499s
   --------------------------------
   Temps demandé : 8.9s
   Boucles prédites : 1.09573e+06
   Durée réelle : 0.208561s
   --------------------------------
   Temps demandé : 9.1s
   Boucles prédites : 1.09678e+06
   Durée réelle : 0.208856s
   --------------------------------
   Temps demandé : 9.3s
   Boucles prédites : 1.09783e+06
   Durée réelle : 0.209115s
   --------------------------------
   Temps demandé : 9.5s
   Boucles prédites : 1.09888e+06
   Durée réelle : 0.209417s
   --------------------------------
   Temps demandé : 9.7s
   Boucles prédites : 1.09993e+06
   Durée réelle : 0.209498s
   --------------------------------
   Temps demandé : 9.9s
   Boucles prédites : 1.10098e+06
   Durée réelle : 0.209651s
   --------------------------------
   ```

## td2_e) Classe consommatrice de CPU durant un temps donné



# TD3

# TD4
