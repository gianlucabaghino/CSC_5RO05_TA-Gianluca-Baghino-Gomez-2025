# CSC_5RO05_TA-Gianluca-Baghino-Gomez-2025
Exercices du Cours RO05 / td-1 td-2 td-3 td-4

# TD1

## a) Gestion simplifiée du temps Posix

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

## b) Classe Chrono

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

## a) Timers avec callback

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

## b) Classe Timer

## c) Fonction simple consommant du CPU

## d) Échantillonage du temps d’exécution d’une fonction

## e) Classe consommatrice de CPU durant un temps donné

# TD3

# TD4
