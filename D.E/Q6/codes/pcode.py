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


dll.gradientDescent.argtypes = [ctypes.c_float]*4 + [ctypes.c_int] 
dll.gradientDescent.restype = ctypes.c_float

dll.free_memory.argtype = [ctypes.POINTER(ctypes.POINTER(ctypes.c_float))] + [ctypes.c_int] 
dll.free_memory.restype = None


#initial conditions

#In the above case, only one half of the solution set is defined ('+' in the '+-' part). 
#For only the other half to be printed ('-' in the '+-' part), uncomment the below lines and comment out the above initial conditions

#initial conditions

h=0.01 #step size
x=1 #inital value of x
n=300 #number of points to plot
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
plt.plot(coordinates[0,:], coordinates[1,:], label="Objective Function", color="b")

#Freeing up malloc'd memory
dll.free_memory(points, n)

x=2 #resetting value of x
min_x = dll.gradientDescent(h, mu, x, tol, n)

print("Results obtained by Gradient Descent Theorem:")
print("minimum value of radius ", min_x)
min_area = (200/min_x) + (2*np.pi*min_x*min_x)
plt.scatter(min_x, min_area, color = 'r', label='Minimum Point' )


'''GEOMETRIC PROGRAMMING'''


# Define the variables
x1 = cp.Variable(pos=True)  # x1 > 0

# Defining the objective function
objective = cp.Minimize(200 / x1 + 2 * np.pi * x1**2)

# Defining the problem
problem = cp.Problem(objective)

# Solving the problem
problem.solve(gp=True)  # Solve as a geometric program

# Display the results
print("Results obtained by Geometric Programming:")
print("Minimized radius value: ", x1.value)
print("Minimized surface area: ", problem.value)

min_area_gp = (200 / x1.value) + (2*np.pi*x1.value*x1.value)

plt.scatter(x1.value , min_area_gp, color = 'yellow', label='Geometric Programming')


#Plot related settings
plt.xlabel("x")
plt.ylabel("y")
plt.grid(True)
plt.legend()
#plt.xlim(0, 5)
#plt.ylim(0, 50)
#plt.show()
plt.savefig("../figs/fig.png")
