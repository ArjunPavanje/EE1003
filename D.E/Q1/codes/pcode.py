import numpy as np
import matplotlib.pyplot as plt
import os
import ctypes

#dll linking -> linking the '.so' file coded from 'C' dynamically to our python program
dll = ctypes.CDLL('./ccode.so')

#specifying argument, and return types for all functions defined in the C program
dll.pointsGet.argtypes = [ctypes.c_float]*5 + [ctypes.c_int] 
dll.pointsGet.restype = ctypes.POINTER(ctypes.POINTER(ctypes.c_float))

dll.free_memory.argtype = [ctypes.POINTER(ctypes.POINTER(ctypes.c_float))] + [ctypes.c_int] 
dll.free_memory.restype = None


#initial conditions
z1_0=1
z2_0=0
z3_0=0

#In the above case, only one half of the solution set is defined ('+' in the '+-' part). 
#For only the other half to be printed ('-' in the '+-' part), uncomment the below lines and comment out the above initial conditions
'''
#initial conditions
z1_0=-1
z2_0=0
z3_0=0
'''
h=0.01 #step size
t=0
n=50

#Implementing modified Euler's method
points=dll.pointsGet(z1_0, z2_0, z3_0, h, t, n)

#Recieving the coordinates and storing them in a numpy array
coordinates=[]
for point in points[:2*n]:
    coordinates.append(np.array([[point[0], point[1]]]).reshape(-1, 1))

coordinates = np.block(coordinates)

#Plotting Points
plt.scatter(coordinates[0,:], coordinates[1,:], label="Sim.", color="blue")
#plt.scatter(coordinates[0,50:], coordinates[1,50:], marker=".", color="blue")

#Freeing up malloc'd memory
dll.free_memory(points, n)

#Plot related settings
plt.xlabel("x")
plt.ylabel("y")
plt.xlim(0,1)
plt.grid(True)
plt.legend()
plt.savefig("../figs/fig1.png")
