import numpy as np
import matplotlib.pyplot as plt
import ctypes
import math

#dll linking -> linking the '.so' file coded from 'C' dynamically to our python program
dll = ctypes.CDLL('./frequency.so')


#specifying argument, and return types for all functions defined in the C program

dll.GetFreq.argtypes = [ctypes.c_int]*2 + [ctypes.c_float]
dll.GetFreq.restype = ctypes.POINTER(ctypes.POINTER(ctypes.c_float))

dll.freeMultiMem.argtypes = [ctypes.POINTER(ctypes.POINTER(ctypes.c_float)), ctypes.c_int]
dll.freeMultiMem.restype = None

n = 20000 # number of times 'm' coin tosses happen
m = 3 # number of coin tosses
p = 0.5 # probability of getting tails

# getting an array of all the points in the plot
pts = dll.GetFreq(n, m, p)

# plotting the differential equation using plt.scatter
coords = []
for pt in pts[:n]:
    coords.append(np.array([[pt[0], pt[1]]]).reshape(-1, 1))

coords_plot = np.block(coords)

plt.scatter(coords_plot[0,:], coords_plot[1,:], marker=".", label = "Sim", color="royalblue")

# plotting the actual probability line
plt.plot(np.linspace(0, n, 1000), [0.125]*1000, linestyle='dashed', color="red")

# freeing the memory of the array 'pts'
dll.freeMultiMem(pts, n)

# saving the plot
plt.savefig('../figs/relative_frequency.png')
plt.show()
