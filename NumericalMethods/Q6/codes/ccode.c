#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

//Impulse Signals
int imp(int n, int n0){
  if(n == n0){
    return 1;
  }
  else{
    return 0;
  }
  return 0;
}
float **trapGet(float h, float x, int n){ //taking initial values of 'x', step size, number of points as input
  float y=(200/x)+(2*M_PI*x*x);
  float **points = (float **) malloc(sizeof(float *)*2*n);
  for (int i=0; i<2*n;i++){
    
    points[i]=(float*)malloc(sizeof(float)*2);
    points[i][0]=x;
    points[i][1]=y;     
    x+=h; //Iteratively increasing 'x' value
    y = (200/x)+(2*M_PI*x*x);
  }
  return points;
}
float gradientDescent(float h, float mu, float x, float tol, int n){ //taking initial values of 'x', step size, number of points as input
  for (int i=0; i<(n);i++){
    if(fabs((4*M_PI*x)-(200/(x*x)) ) < tol) break;
    x = x*(1-4*mu*M_PI) + ((200*mu)/(x*x));
  }
  return x;
}

//To free up used memory
void free_memory(float **points, int n){
  for(int i=0;i<2*n; i++){
    free(points[i]);
  }
  free(points);
}
