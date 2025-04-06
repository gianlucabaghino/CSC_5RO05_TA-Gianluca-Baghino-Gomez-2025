import matplotlib.pyplot as plt

# Data (you can load this from the file or paste it directly)
nLoops = [100, 1000, 10000, 100000, 1000000, 10000000, 100000000, 1000000000]
temps = [0.007656, 0.060156, 0.585469, 5.915, 58.5209, 584.381, 5843.03, 58428.2]

# Plotting the data
plt.plot(nLoops, temps, marker='o')
plt.xscale('log')  # Logarithmic scale for better visualization
plt.yscale('log')  # Logarithmic scale for better visualization
plt.xlabel('nLoops')
plt.ylabel('Temps d\'exécution (millisecondes)')
plt.title('Temps d\'exécution en fonction du nombre de boucles')
plt.grid(True)

# Show the plot
plt.show()
