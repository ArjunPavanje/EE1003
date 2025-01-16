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
  float y= (x*x) - (25*x) + 156;
  float **points = (float **) malloc(sizeof(float *)*2*n);
  for (int i=0; i<2*n;i++){
    points[i]=(float*)malloc(sizeof(float)*2);
    points[i][0]=x;
    points[i][1]=y;     
    x+=h; //Iteratively increasing 'x' value
    y = (x*x) - (25*x) + 156;
  }
  return points;
}
float newtonRalphson(float x, int n){ //taking initial values of 'x', step size, number of points as input
  for (int i=0; i<(n);i++){
    x = x - ( (x*x - 25*x +156) / (2*x - 25));
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
