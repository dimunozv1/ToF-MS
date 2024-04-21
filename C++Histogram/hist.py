
import matplotlib.pyplot as plt
import numpy as np

# Read data from file
with open('data-dist-normal.txt', 'r') as f:
    data = [float(line.strip()) for line in f]
print(np.min(data), np.max(data))
# Plot histogram
plt.hist(data, bins=80)
plt.xlabel('Value')
plt.ylabel('Frequency')
plt.title('Histogram from output.txt')
plt.show()