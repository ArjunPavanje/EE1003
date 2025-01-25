import matplotlib.pyplot as plt
import scipy.stats  
import math 
import numpy as np

# defining the piecewise cdf function
def cdf(n):
    if (n < 0):
        return 0
    elif (0 <= n < 1):
        return 0.125
    elif (1 <= n < 2):
        return 0.5 
    elif (2 <= n < 3):
        return 0.875
    else:
        return 1


x = np.linspace(0, 5, 1000)
y = [cdf(n) for n in x]

#Plotting
plt.figure()
plt.plot(x, y, label="CDF $F_{X}(n)$")
plt.legend()
plt.savefig("../figs/cdf.png")
plt.show()
