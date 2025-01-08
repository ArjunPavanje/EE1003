#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/*
*Trapezoid law for solving differential equations

y'=-y+e^x
-> dy = -y.dx + e^x.dx
-> discretizing the steps (x_{n+1} = x_n + h), integrating from n to n-1
-> we get y(n+1)-y(n)=1/2*h*(y(n+1)+y(n))+1/2*h*(e^(x(n+1))+e^(e(n)))
* R.H.S comes from trapezoid method of integration as used in the previous question
* We thus obtain general difference equation for y(n)
*/

float **simGet(float h, float y, float x, int n){ //taking initial values of 'x', step size, number of points as input

  float **points = (float **) malloc(sizeof(float *)*2*n);
  for (int i=0; i<2*n; i++){
    points[i]=(float*)malloc(sizeof(float)*2);
    points[i][0]=x;
    points[i][1]=exp(x)/2;    
    y=y-h*((y*y + y + 1)/(x*x + x +1));
    x+=h; //Iteratively increasing 'x' value
  }
  return points;
}// As both simulated and theoretical plot are symmetric, only one side (+x axis) side is calculated.
//To free up used memory
void free_memory(float **points, int n){
  for(int i=0;i<2*n; i++){
    free(points[i]);
  }
  free(points);
}
