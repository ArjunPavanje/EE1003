import numpy as np
import matplotlib.pyplot as plt

# Load the points from the text file
points = np.loadtxt("line_points.txt", delimiter=',', max_rows=len(list(open("./line_points.txt")))-2)

# Extract the x and y coordinates for the points on the line
x1 = points[:21, 0]
y1 = points[:21, 1]

# Load the direction vector and normal vector from the last two lines
dirvector = np.loadtxt("line_points.txt", delimiter=',', skiprows=len(list(open("./line_points.txt")))-2, max_rows=1)
normvector = np.loadtxt("line_points.txt", delimiter=',', skiprows=len(list(open("./line_points.txt")))-1, max_rows=1)

# Normalize the direction and normal vectors to unit vectors
dirvector_magnitude = np.linalg.norm(dirvector)
normvector_magnitude = np.linalg.norm(normvector)

# Normalize both vectors to unit vectors
dirvector_unit = dirvector / dirvector_magnitude
normvector_unit = normvector / normvector_magnitude

# Define a scaling factor for the vectors (to make them visible)
scale_factor = 1.0

# Scale both vectors to the same length (common length)
dirvector_scaled = dirvector_unit * scale_factor
normvector_scaled = normvector_unit * scale_factor

# Set the origin to the center (0,0)
origin = np.array([0, 0])

# Create a figure
plt.figure()

# Plot the line points (blue line)
plt.plot(x1, y1, label='Line segment', color='blue')

# Plot the scaled direction vector (green) from the origin
plt.quiver(origin[0], origin[1], dirvector_scaled[0], dirvector_scaled[1], angles='xy', scale_units='xy', scale=1, color='green', label='Direction vector')

# Plot the scaled normal vector (red) from the origin
plt.quiver(origin[0], origin[1], normvector_scaled[0], normvector_scaled[1], angles='xy', scale_units='xy', scale=1, color='red', label='Normal vector')

# Set equal aspect ratio to ensure correct proportions
plt.gca().set_aspect('equal', adjustable='box')

# Define limits for x and y axis to keep origin centered and vectors in frame
vector_max = max(np.abs(dirvector_scaled).max(), np.abs(normvector_scaled).max())
plt.xlim(-vector_max * 1.5, vector_max * 1.5)
plt.ylim(-vector_max * 1.5, vector_max * 1.5)

# Add labels, title, and grid
plt.xlabel("x")
plt.ylabel("y")
plt.title("Line and Vectors: Direction and Normal")
plt.grid(True)
plt.legend()

plt.savefig('../figs/fig.png')
plt.show()

