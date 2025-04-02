# CSC_5RO05_TA-Gianluca-Baghino-Gomez-2025
Exercices du Cours RO05 / td-1 td-2 td-3 td-4

# TD1

## a) Gestion simplifiée du temps Posix

╭─╾[ensta-ThinkCentre-M800:/home/g.baghino]╼╼╼╼╼╼╼╼╼╼╼╼╼╼╼╼╼╼╼╼╼╼╼╼╼╼╼╼╼╼╼╼╼╼╼╼╼╼╼╼╼╼╼╼╼╼╼╼
╰─╾[g.baghino]╼ cd MyProject/src/
╭─╾[ensta-ThinkCentre-M800:/home/g.baghino/MyProject/src]╼╼╼╼╼╼╼╼╼╼╼╼╼╼╼╼╼╼╼╼╼╼╼╼╼╼╼╼╼╼╼╼╼╼
╰─╾[g.baghino]╼ # Compile MainPosix.cpp and TimespecUtils.cpp for ARM
arm-linux-g++ -Wall -Wextra MainPosix.cpp TimespecUtils.cpp -o td1a
╭─╾[ensta-ThinkCentre-M800:/home/g.baghino/MyProject/src]╼╼╼╼╼╼╼╼╼╼╼╼╼╼╼╼╼╼╼╼╼╼╼╼╼╼╼╼╼╼╼╼╼╼
╰─╾[g.baghino]╼ rsync -avz td1a root@192.168.50.43:
sending incremental file list
td1a

sent 5,164 bytes  received 35 bytes  3,466.00 bytes/sec
total size is 14,120  speedup is 2.72
╭─╾[ensta-ThinkCentre-M800:/home/g.baghino/MyProject/src]╼╼╼╼╼╼╼╼╼╼╼╼╼╼╼╼╼╼╼╼╼╼╼╼╼╼╼╼╼╼╼╼╼╼
╰─╾[g.baghino]╼ ssh root@192.168.50.43
# ./td1a
2500.5 ms en secondes : 2500.5 ms
Temps actuel : 534058 secondes et 333814554 nanosecondes
Temps négatif : -3 secondes et 499500000 nanosecondes
Temps additionné : 6 secondes et 500000 nanosecondes
Temps soustrait : 0 secondes et 999500000 nanosecondes
Somme des temps : 6 secondes et 500000 nanosecondes
Différence des temps : 0 secondes et 999500000 nanosecondes
# 

## b)

# TD2

# TD3

# TD4
