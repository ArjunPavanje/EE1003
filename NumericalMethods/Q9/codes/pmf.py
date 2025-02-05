import matplotlib.pyplot as plt
import scipy.stats  
import math 
import numpy as np

import ctypes

#dll linking -> linking the '.so' file coded from 'C' dynamically to our python program
dll = ctypes.CDLL('./frequency.so')


#specifying argument, and return types for all functions defined in the C program

dll.simPMF.argtypes = [ctypes.c_int]*2 + [ctypes.c_float]
dll.simPMF.restype = ctypes.POINTER(ctypes.POINTER(ctypes.c_float))   

dll.freeSingle.argtypes = [ctypes.POINTER(ctypes.POINTER(ctypes.c_float))]  + [ctypes.c_int]
dll.freeSingle.restype = None

# Generating Theoretical PMF using scipy's binom.pmf package
def pmf(n, p):
    
    x = np.arange(0, n+1) # Getting an array with values 0, 1, .. n (not n+1, it works similar to range())

    y = scipy.stats.binom.pmf(x, n, p)

    return x, y

n = 200000
m = 3  # Number of tosses
p = 0.5 # Possibility of heads
    
pts = dll.simPMF(m, n, p)

# plotting the differential equation using plt.scatter
coords = []
for pt in pts[:(m+1)]:
    coords.append(np.array([[pt[0], pt[1]]]).reshape(-1, 1))

coords_plot = np.block(coords)



dll.freeSingle(pts, m)
x, y = pmf(m, p)
 

'''
mean = m*p 
variance = m*p*(1-p)

x1 = np.linspace(0, m, 10000)
y1 = np.exp(-np.power(x1 - mean, 2)/(2 * variance))/math.sqrt(2*np.pi*variance)
'''


# Plot the pmf
plt.figure()

markerline, stemlines, _ = plt.stem(coords_plot[0,:], coords_plot[1,:], 'orange', label = "Sim")
plt.setp(markerline, 'markersize', 8)

plt.stem(x, y, label = "Theory")

#plt.plot(x1, y1, label="Gaussian")
plt.legend()
plt.savefig("../figs/pmf_1.png")
#plt.show()
