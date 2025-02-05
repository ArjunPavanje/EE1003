import numpy as np
import matplotlib.pyplot as plt
import os
import ctypes

#dll linking -> linking the '.so' file coded from 'C' dynamically to our python program
dll = ctypes.CDLL('./ccode.so')

#specifying argument, and return types for all functions defined in the C program
dll.pointsGet.argtypes = [ctypes.c_float]*3 + [ctypes.c_int] 
dll.pointsGet.restype = ctypes.POINTER(ctypes.POINTER(ctypes.c_float))

dll.free_memory.argtype = [ctypes.POINTER(ctypes.POINTER(ctypes.c_float))] + [ctypes.c_int] 
dll.free_memory.restype = None


#initial conditions

#In the above case, only one half of the solution set is defined ('+' in the '+-' part). 
#For only the other half to be printed ('-' in the '+-' part), uncomment the below lines and comment out the above initial conditions

'''THEORETICAL SOLUTION'''

x_linspace = np.linspace(-5, 5, 1000)
y_linspace = -x_linspace/(1+x_linspace) 

#Plotting theoretical solution
plt.plot(x_linspace, y_linspace, color = 'blue', label = "Theory")

'''COMPUTATIONAL SOLUTION'''
#initial conditions

h=0.01 #step size
x=-5 #inital value of x
y=-1.25 #initla 'y' value
n=1000 #number of points to plot

#Implementing modified Euler's method
points=dll.pointsGet(h, y, x, n)

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

#initial conditions

x=-0.9 #inital value of x
y=9
#Implementing modified Euler's method
points=dll.pointsGet(h, y, x, n)

#Recieving the coordinates and storing them in a numpy array
coordinates=[]
for point in points[:2*n]:
    coordinates.append(np.array([[point[0], point[1]]]).reshape(-1, 1))

coordinates = np.block(coordinates)

#Plotting Points
plt.scatter(coordinates[0,:], coordinates[1,:], color="red")
#plt.scatter(coordinates[0,50:], coordinates[1,50:], marker=".", color="blue")

#Freeing up malloc'd memory
dll.free_memory(points, n)
#Plot related settings
plt.xlabel("x")
plt.ylabel("y")
plt.xlim(-5, 5)
plt.ylim(-10,10)
plt.grid(True)
plt.legend()
plt.savefig("../figs/fig.png")
