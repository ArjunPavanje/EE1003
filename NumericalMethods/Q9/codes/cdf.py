import matplotlib.pyplot as plt
import scipy.stats  
import math 
import numpy as np
import ctypes

#dll linking -> linking the '.so' file coded from 'C' dynamically to our python program
dll = ctypes.CDLL('./frequency.so')


#specifying argument, and return types for all functions defined in the C program

dll.simCDF.argtypes = [ctypes.c_int]*2 + [ctypes.c_float]
dll.simCDF.restype = ctypes.POINTER(ctypes.POINTER(ctypes.c_float))   

dll.freeSingle.argtypes = [ctypes.POINTER(ctypes.POINTER(ctypes.c_float))]  + [ctypes.c_int]
dll.freeSingle.restype = None


# Generating Theoretical CDF using scipy's binom.cdf package
def cdf(n, p):
    
    x = np.arange(0, n+1) # Getting an array with values 0, 1, .. n (not n+1, it works similar to range())

    y = scipy.stats.binom.cdf(x, n, p)

    return x, y

m = 3 # number of tosses
n = 200000 # Number of trials
p = 0.5 # probability of head

pts = dll.simCDF(m, n, p)

# plotting the differential equation using plt.scatter
coords = []
for pt in pts[:(m+1)]:
    coords.append(np.array([[pt[0], pt[1]]]).reshape(-1, 1))

coords_plot = np.block(coords)



dll.freeSingle(pts, m)


x, y = cdf(m, p)
#Plotting
plt.figure()

markerline, stemlines, _ = plt.stem(coords_plot[0,:], coords_plot[1,:], 'orange', label = "Sim") 
plt.setp(markerline, 'markersize', 8)

plt.stem(x, y, label="Theory")
plt.legend()
plt.savefig("../figs/cdf.png")
plt.show()
