import numpy as np
import matplotlib.pyplot as plt
import os
import ctypes

#dll linking -> linking the '.so' file coded from 'C' dynamically to our python program
dll = ctypes.CDLL('./ccode.so')

#specifying argument, and return types for all functions defined in the C program
dll.pointsGet.argtypes = [ctypes.c_float]*2 + [ctypes.c_int] 
dll.pointsGet.restype = ctypes.POINTER(ctypes.POINTER(ctypes.c_float))

dll.area.argtypes = [ctypes.c_float]*2 + [ctypes.c_int]
dll.area.restype = ctypes.c_float

dll.xparabola_gen.argtypes = [ctypes.c_float]*3 + [ctypes.c_int] 
dll.xparabola_gen.restype = ctypes.POINTER(ctypes.POINTER(ctypes.c_float))

dll.free_memory.argtype = [ctypes.POINTER(ctypes.POINTER(ctypes.c_float))] + [ctypes.c_int] 
dll.free_memory.restype = None



#In the above case, only one half of the solution set is defined ('+' in the '+-' part). 
#For only the other half to be printed ('-' in the '+-' part), uncomment the below lines and comment out the above initial conditions

#initial conditions

h=0.0001 #step size
x=0 #inital value of x
n=1000 #number of points to plot
a=0.25 #'a' in 4ay=x^2
n_area=10000
'''COMPUTATIONAL SOLUTION'''

ar=dll.area(x, h, n_area)
print("Area enclosed between the two curves: ", ar)


'''PLOT'''
h=0.01
'''PARABOLA'''
#Implementing modified Euler's method
points=dll.xparabola_gen(x, h, a, n)

#Recieving the coordinates and storing them in a numpy array
coordinates=[]
for point in points[:2*n]:
    coordinates.append(np.array([[point[0], point[1]]]).reshape(-1, 1))

coordinates = np.block(coordinates)

'''LINE'''
#Implementing modified Euler's method
points1=dll.pointsGet(h, x, n)

#Recieving the coordinates and storing them in a numpy array
coordinates1=[]
for point1 in points1[:2*n]:
    coordinates1.append(np.array([[point1[0], point1[1]]]).reshape(-1, 1))

coordinates1 = np.block(coordinates1)

#Plotting Points
plt.scatter(coordinates1[0,:], coordinates1[1,:], label="Line" ,color="blue")
plt.fill_between(coordinates[0, :], coordinates[1, :], coordinates1[1, :], where=(coordinates1[1, :] > coordinates[1, :]), color="yellow", alpha=0.5)
plt.scatter(coordinates[0,:], coordinates[1,:], label="Parabola", color="red")

#Freeing up malloc'd memory
dll.free_memory(points1, n)
dll.free_memory(points, n)

#Plot related settings
plt.xlabel("x")
plt.ylabel("y")
plt.xlim(-2, 2)
plt.ylim(0,3)
plt.legend()
plt.grid(True)
plt.savefig("../figs/fig.png")


