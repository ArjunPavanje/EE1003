import numpy as np
import matplotlib.pyplot as plt
import os
import ctypes

#dll linking -> linking the '.so' file coded from 'C' dynamically to our python program
dll = ctypes.CDLL('./ccode.so')

#specifying argument, and return types for all functions defined in the C program

dll.trapGet.argtypes = [ctypes.c_float]*3 + [ctypes.c_int] 
dll.trapGet.restype = ctypes.POINTER(ctypes.POINTER(ctypes.c_float))


dll.bilGet.argtypes = [ctypes.c_float]*3 + [ctypes.c_int] 
dll.bilGet.restype = ctypes.POINTER(ctypes.POINTER(ctypes.c_float))

dll.free_memory.argtype = [ctypes.POINTER(ctypes.POINTER(ctypes.c_float))] + [ctypes.c_int] 
dll.free_memory.restype = None


#initial conditions

#In the above case, only one half of the solution set is defined ('+' in the '+-' part). 
#For only the other half to be printed ('-' in the '+-' part), uncomment the below lines and comment out the above initial conditions

'''THEORETICAL SOLUTION'''

#Function obtained by laplace transform is,
# y = (e^x + e^-x)/2


x1=np.linspace(0, 10)
y1=(np.exp(x1) + np.exp(-x1))/2
plt.plot(x1, y1, color='r', label='Theory')


'''COMPUTATIONAL SOLUTION (TRAPEZOIDAL LAW)'''
#initial conditions

h=0.01 #step size
x=0 #inital value of x
y=1 #initla 'y' value
n=1000 #number of points to plot

#Implementing modified Trapezoidal method
points=dll.trapGet(h, y, x, n)

#Recieving the coordinates and storing them in a numpy array
coordinates=[]
for point in points[:2*n]:
    coordinates.append(np.array([[point[0], point[1]]]).reshape(-1, 1))

coordinates = np.block(coordinates)

#Plotting Pointplt.scatter(coordinates[0,:], coordinates[1,:], label="sim", color="red")
plt.scatter(coordinates[0,:], coordinates[1,:], label="Sim-1", color="b")

#Freeing up malloc'd memory
dll.free_memory(points, n)

'''COMPUTATIONAL METHOD (BILINEAR TRANSFORM)'''
#initial conditions

h=0.01 #step size
x=0 #inital value of x
y=1 #initla 'y' value
n=1000 #number of points to plot#Implementing modified Trapezoidal method
points=dll.bilGet(h, y, x, n)

#Recieving the coordinates and storing them in a numpy array
coordinates=[]
for point in points[:2*n]:
    coordinates.append(np.array([[point[0], point[1]]]).reshape(-1, 1))

coordinates = np.block(coordinates)

#Plotting Pointplt.scatter(coordinates[0,:], coordinates[1,:], label="sim", color="red")
plt.scatter(coordinates[0,:], coordinates[1,:], label="Sim-2", color="g")

#Freeing up malloc'd memory
dll.free_memory(points, n)

#Plot related settings
plt.xlabel("x")
plt.ylabel("y")
plt.grid(True)
plt.legend()
plt.xlim(0, 5)
plt.ylim(0, 50)#plt.show()
plt.savefig("../figs/fig.png")
