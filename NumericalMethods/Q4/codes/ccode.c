#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/*
* Trapezoid method to calculate definite integrals
-> Divide the points on 'x' axis between the two target points into equidistant number of some 'n' points
-> Draw a line along the y-axis for each of these lines from the 'x' axis to where the cut the curve
-> We thus obtain 'n-1' trapeziums if we join the top parts of each adjascent line
-> Summing up all the areas gives us a pretty close approximation of the integral especially when 'n' value is very large
*/

//Generating a parabola of the form x^2=4ay
float **xparabola_gen(float x, float h, float a, int num_points){
  //Making array
  float **points = (float **) malloc(sizeof(float *)*2*num_points);
	for(int i=0;i<=num_points;i++){
    points[i]=(float*)malloc(sizeof(float)*2);
    points[num_points+i]=(float*)malloc(sizeof(float)*2);
		points[i][0]=x;
		points[i][1]=(x*x)/(4*a);
    points[num_points+i][0]=-x;
    points[num_points+i][1]=(x*x)/(4*a);
    x=x+h;
	}
  return points;
}
float **pointsGet(float h, float x, int n){ //taking initial values of 'x', step size, number of points as input

  float **points = (float **) malloc(sizeof(float *)*2*n);
  for (int i=0; i<n; i++){
    points[i]=(float*)malloc(sizeof(float)*2);
    points[n+i]=(float*)malloc(sizeof(float)*2);

    points[i][0]=x;
    points[i][1]=x;
    points[n+i][0]=-x;
    points[n+i][1]=x;    
    x+=h; //Iteratively increasing 'x' value
  }
  return points;
}
float area(float x, float h, int n){
  float A=0;
  for (int i=0; i<n; i++){
    A+=(h*(x-x*x))+0.5*(h*h*(1-2*x));
    x+=h;
  }
  return 2*A;
}
// As both simulated and theoretical plot are symmetric, only one side (+x axis) side is calculated.
//To free up used memory
void free_memory(float **points, int n){
  for(int i=0;i<2*n; i++){
    free(points[i]);
  }
  free(points);
}
