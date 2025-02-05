import numpy as np
import matplotlib.pyplot as plt
import os
import ctypes

#dll linking -> linking the '.so' file coded from 'C' dynamically to our python program
dll = ctypes.CDLL('./ccode.so')

#specifying argument, and return types for all functions defined in the C program
dll.pointsGet.argtypes = [ctypes.c_float]*2 + [ctypes.c_int] 
dll.pointsGet.restype = ctypes.POINTER(ctypes.POINTER(ctypes.c_float))

dll.theoreticalPoints.argtypes = [ctypes.c_float] + [ctypes.c_int] 
dll.theoreticalPoints.restype = ctypes.POINTER(ctypes.POINTER(ctypes.c_float))

dll.free_memory.argtype = [ctypes.POINTER(ctypes.POINTER(ctypes.c_float))] + [ctypes.c_int] 
dll.free_memory.restype = None


#initial conditions

#In the above case, only one half of the solution set is defined ('+' in the '+-' part). 
#For only the other half to be printed ('-' in the '+-' part), uncomment the below lines and comment out the above initial conditions

#initial conditions

h=0.01 #step size
x=0.1 #inital value of x
n=700 #number of points to plot
'''THEORETICAL SOLUTION'''
#Plotting Theoretical Data
points=dll.theoreticalPoints(h, n)

#Recieving the coordinates and storing them in a numpy array
coordinates=[]
for point in points[:2*n]:
    coordinates.append(np.array([[point[0], point[1]]]).reshape(-1, 1))

coordinates = np.block(coordinates)

#Plotting Points
plt.scatter(coordinates[0,:], coordinates[1,:], label="theory", color="blue")

#Freeing up malloc'd memory
dll.free_memory(points, n)

'''COMPUTATIONAL SOLUTION'''

#Implementing modified Euler's method
points=dll.pointsGet(h, t, n)

#Recieving the coordinates and storing them in a numpy array
coordinates=[]
for point in points[:2*n]:
    coordinates.append(np.array([[point[0], point[1]]]).reshape(-1, 1))

coordinates = np.block(coordinates)

#Plotting Points
plt.scatter(coordinates[0,:], coordinates[1,:], label="sim", color="red")
#plt.scatter(coordinates[0,50:], coordinates[1,50:], marker=".", color="blue")

#Freeing up malloc'd memory
dll.free_memory(points, n)


#Plot related settings
plt.xlabel("x")
plt.ylabel("y")
plt.xlim(-10, 10)
plt.grid(True)
plt.legend()
plt.savefig("../figs/fig.png")
