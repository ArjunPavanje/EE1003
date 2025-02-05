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
float **trapGet(float h, float y, float x, int n){ //taking initial values of 'x', step size, number of points as input

  float **points = (float **) malloc(sizeof(float *)*2*n);
  for (int i=0; i<2*n;i++){
    points[i]=(float*)malloc(sizeof(float)*2);
    points[i][0]=x;
    points[i][1]=y;     
    x+=h; //Iteratively increasing 'x' value
    y = (y)*((2-h)/(2+h)) + (2*exp(x)/(2+h))*(exp(h) - 1);
  }
  return points;
}
float **bilGet(float h, float y, float x, int n){ //taking initial values of 'x', step size, number of points as input
  float y0 = y;
  float **points = (float **) malloc(sizeof(float *)*2*n);
  points[0]=(float*)malloc(sizeof(float)*2);
  points[0][0]=x;
  points[0][1]=y0;
  float alpha = (2-h)/(2+h);
  for (int i=1; i<(2*n);i++){
    points[i]=(float*)malloc(sizeof(float)*2);
    points[i][0]=x;
    points[i][1]=y;     
    x+=h; //Iteratively increasing 'x' value
    y = (y*alpha) + h*((y0 - 0.5)/(2+h))*(imp(i, 1)) + (1/(2*(2+h)))* ( (1 + alpha - pow(alpha, 2) - pow(alpha, 3)  )*pow(alpha, -i) + pow(alpha, 2)*imp(i, 1) );
  }
  return points;
}



//To free up used memory
void free_memory(float **points, int n){
  for(int i=0;i<2*n; i++){
    free(points[i]);
  }
  free(points);
}
