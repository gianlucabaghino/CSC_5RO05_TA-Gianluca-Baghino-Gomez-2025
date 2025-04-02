# CSC_5RO05_TA-Gianluca-Baghino-Gomez-2025
Exercices du Cours RO05 / td-1 td-2 td-3 td-4

# TD1

## a) Gestion simplifiée du temps Posix

To compile it, follow these steps:

1. **Navigate to the project directory:**
   ```sh
   cd MyProject/src/

2. Compile the source files

arm-linux-g++ -Wall -Wextra MainPosix.cpp TimespecUtils.cpp -o td1a

Transfer the compiled file to your ARM device: Use rsync to send the compiled file:

rsync -avz td1a root@192.168.50.43:

This will transfer the file to the target machine with the IP address 192.168.50.43.

SSH into the ARM device: Connect to the ARM device via SSH:

ssh root@192.168.50.43

Run the compiled program: Once logged in, you can execute the program:

    ./td1a

Example Output:

2500.5 ms en secondes : 2500.5 ms
Temps actuel : 534058 secondes et 333814554 nanosecondes
Temps négatif : -3 secondes et 499500000 nanosecondes
Temps additionné : 6 secondes et 500000 nanosecondes
Temps soustrait : 0 secondes et 999500000 nanosecondes
Somme des temps : 6 secondes et 500000 nanosecondes
Différence des temps : 0 secondes et 999500000 nanosecondes


## b)

# TD2

# TD3

# TD4
