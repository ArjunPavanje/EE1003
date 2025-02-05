import numpy as np
import cvxpy as cp
import matplotlib.pyplot as plt
import os
import ctypes

#dll linking -> linking the '.so' file coded from 'C' dynamically to our python program
dll = ctypes.CDLL('./ccode.so')

#specifying argument, and return types for all functions defined in the C program

dll.trapGet.argtypes = [ctypes.c_float]*2 + [ctypes.c_int] 
dll.trapGet.restype = ctypes.POINTER(ctypes.POINTER(ctypes.c_float))


dll.newtonRalphson.argtypes = [ctypes.c_float] + [ctypes.c_int] 
dll.newtonRalphson.restype = ctypes.c_float

dll.free_memory.argtype = [ctypes.POINTER(ctypes.POINTER(ctypes.c_float))] + [ctypes.c_int] 
dll.free_memory.restype = None


#initial conditions

#In the above case, only one half of the solution set is defined ('+' in the '+-' part). 
#For only the other half to be printed ('-' in the '+-' part), uncomment the below lines and comment out the above initial conditions

#initial conditions

h=0.01 #step size
x=10 #inital value of x
n=200 #number of points to plot
tol = 0.00001 #tolerence
mu=0.01 #stepsize

#Implementing modified Trapezoidal method
points=dll.trapGet(h, x, n)

#Recieving the coordinates and storing them in a numpy array
coordinates=[]
for point in points[:2*n]:
    coordinates.append(np.array([[point[0], point[1]]]).reshape(-1, 1))

coordinates = np.block(coordinates)

#Plotting Pointplt.scatter(coordinates[0,:], coordinates[1,:], label="sim", color="red")
plt.plot(coordinates[0,:], coordinates[1,:], label="Quadratic Equation", color="b")

#Freeing up malloc'd memory
dll.free_memory(points, n)

x=10 #resetting value of x
x1 = dll.newtonRalphson(x, n)
x=30
x2 = dll.newtonRalphson(x, n)

y = (0, 0)
x_nr = (x1, x2)

plt.scatter(x_nr, y, color = 'r', label = "Roots")

#Data obtained by eigenvalues method

x1_eigen = 11.999964 
x2_eigen = 13.000036
x_eigen = (x1_eigen, x2_eigen)

plt.scatter(x_eigen, y, color='orange', label = "Roots")
#Plot related settings
plt.xlabel("x")
plt.ylabel("y")
plt.grid(True)
plt.legend()
#plt.xlim(0, 5)
#plt.ylim(0, 50)
#plt.show()
plt.savefig("../figs/fig.png")
