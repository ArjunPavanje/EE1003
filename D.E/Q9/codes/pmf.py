import matplotlib.pyplot as plt
import scipy.stats  
import math 
import numpy as np

# Function to calculate and return pmf
def pmf(n, p):
    
    x = np.arange(0, n+1) # Getting an array with values 0, 1, .. n (not n+1, it works similar to range())

    y = scipy.stats.binom.pmf(x, n, p)

    return x, y

m = 100  # Number of tosses
p = 0.5 # Possibility of heads

x, y = pmf(m, p)
 

mean = m*p 
variance = m*p*(1-p)

x1 = np.linspace(0, m, 10000)
y1 = np.exp(-np.power(x1 - mean, 2)/(2 * variance))/math.sqrt(2*np.pi*variance)

# Plot the pmf
plt.figure()

plt.stem(x, y, label = "PMF $P_{X}(n)$ ")
plt.plot(x1, y1, label="Gaussian")
plt.legend()
plt.savefig("../figs/pmf5.png")
plt.show()
