import numpy as np
import cvxpy as cp
import matplotlib.pyplot as plt
import os
import ctypes

#dll linking -> linking the '.so' file coded from 'C' dynamically to our python program
dll = ctypes.CDLL('./ccode.so')

#specifying argument, and return types for all functions defined in the C program

dll.LUsolver.argtypes = [ctypes.c_int]
dll.LUsolver.restype = ctypes.POINTER(ctypes.POINTER(ctypes.c_float))


# Plotting the lines

x = np.linspace(-5, 5, 1000)
y1 = 6 - 2*x
y2 = 2*x - 2

plt.plot(x, y1, color='r', label = "2x + y = 6")
plt.plot(x, y2, color = 'b', label = '2x - y = 2')


# Verifying with LU decomposition
i = dll.LUsolver(2)
plt.scatter(i[0][0], i[1][0], color='g', label='Solution')

#initial conditions

#Plot related settings
plt.xlabel("x")
plt.ylabel("y")
plt.grid(True)
plt.legend()
#plt.xlim(0, 5)
#plt.ylim(0, 50)
#plt.show()
plt.savefig("../figs/fig.png")
