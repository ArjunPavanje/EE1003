#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/*
* Euler's Method to solve linear first order D.E's:


By using first principle of derivatives and given data we can write
* y(x+h)=y(x)+h(y'(x))
Repeatedly running above equation gives requierd points.

*/
float **xparabola_gen(float a, int num_points){
  float **points = (float **) malloc(sizeof(float *)*2*num_points);
  float h=0.01;
  float x=0;
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
float **pointsGet(float h, float y, float x, int n){ //taking initial values of 'x', step size, number of points as input

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
