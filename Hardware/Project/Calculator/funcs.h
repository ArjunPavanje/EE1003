#include<stdlib.h>

#define PI 3.14159265358979323846
// Fixed step size
#define H 0.01

// Function for first derivative (y' = z)
double f1(double x, double y, double z) {
    return z;
}

// Function for second derivative (z' = -y)
double f2(double x, double y, double z) {
    return -y;
}

// Function to normalize angle to [0, 2π)
double normalize_angle(double angle) {
    // First, get the remainder when divided by 2π
    while (angle >= 2*PI) {
        angle -= 2*PI;
    }
    while (angle < 0) {
        angle += 2*PI;
    }
    return angle;
}

// Function to calculate sine using RK4 method
double sin_rk4(double x_target) {
    double x, y, z;
    double k1, k2, k3, k4, l1, l2, l3, l4;
    
    // Normalize the target angle to [0, 2π)
    x_target = normalize_angle(x_target);
    
    // Initial conditions for y = sin(x)
    x = 0.0;    // Starting x value
    y = 0.0;    // y(0) = 0
    z = 1.0;    // y'(0) = 1
    
    // RK4 method to solve the differential equation
    while(x < x_target) {
        // Ensure we don't overshoot the target
        if (x + H > x_target) {
            // Adjust the last step size to hit x_target exactly
            double last_h = x_target - x;
            
            // Calculate k values for y
            k1 = last_h * f1(x, y, z);
            l1 = last_h * f2(x, y, z);
            
            k2 = last_h * f1(x + last_h/2, y + k1/2, z + l1/2);
            l2 = last_h * f2(x + last_h/2, y + k1/2, z + l1/2);
            
            k3 = last_h * f1(x + last_h/2, y + k2/2, z + l2/2);
            l3 = last_h * f2(x + last_h/2, y + k2/2, z + l2/2);
            
            k4 = last_h * f1(x + last_h, y + k3, z + l3);
            l4 = last_h * f2(x + last_h, y + k3, z + l3);
            
            // Update y and z
            y = y + (k1 + 2*k2 + 2*k3 + k4)/6;
            z = z + (l1 + 2*l2 + 2*l3 + l4)/6;
            
            // Update x to target
            x = x_target;
        } else {
            // Regular step
            // Calculate k values for y
            k1 = H * f1(x, y, z);
            l1 = H * f2(x, y, z);
            
            k2 = H * f1(x + H/2, y + k1/2, z + l1/2);
            l2 = H * f2(x + H/2, y + k1/2, z + l1/2);
            
            k3 = H * f1(x + H/2, y + k2/2, z + l2/2);
            l3 = H * f2(x + H/2, y + k2/2, z + l2/2);
            
            k4 = H * f1(x + H, y + k3, z + l3);
            l4 = H * f2(x + H, y + k3, z + l3);
            
            // Update y and z
            y = y + (k1 + 2*k2 + 2*k3 + k4)/6;
            z = z + (l1 + 2*l2 + 2*l3 + l4)/6;
            
            // Update x
            x = x + H;
        }
    }
    
    return y;
}
