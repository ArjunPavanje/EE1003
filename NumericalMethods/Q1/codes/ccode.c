#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/*
* Modified Euler's Method for higher order D.E.s :

Here are a few notations used,
*z1=y(t)
*z2=y'(t)
*z3=y''(t)
*z4=y'''(t)

By using first principle of derivatives and given data we can write
* z3(t+h)=z3(t) (+-)h*sqrt(z3^3 + z2^4 + z1^5) !!!!
* z2(t+h)=z2(t)+h*(z3(t))
* z1(t+h)=z1(t)+h*(z2(t))

*/

float **pointsGet(float z1_0, float z2_0, float z3_0, float h, float t, int n){ //taking initial values of z1, z2, z3, step value, 't' as input

  float **points = (float **) malloc(sizeof(float *)*2*n);
  float z1_init=z1_0, z2_init=z2_0, z3_init=z3_0;
  float z1=z1_0, z2=z2_0, z3=z3_0;

  //For the '+' case in highlighted Equation
  for (int i=0; i<n; i++){
    z3=(z3_0)-h*(sqrt(-(pow(z3_0,3) + pow(z2_0,4) + pow(z1_0,5))));
    z2=(z2_0)+h*(z3_0);
    z1=(z1_0)+h*(z2_0);

    points[i]=(float*)malloc(sizeof(float)*2*n);
    points[i][0]=t;
    points[i][1]=z1;
    z1_0=z1;
    z2_0=z2;
    z3_0=z3;
    t+=h;
  }

  //Resetting initial conditions
  t=0;
  z1_0=z1_init;
  z2_0=z2_init;
  z3_0=z3_init;

  //For the '-' case in highlighted Equation
  for (int i=n; i<2*n; i++){
    z3=(z3_0)-h*(sqrt((pow(z3_0,3) + pow(z2_0,4) + pow(z1_0,5))));
    z2=(z2_0)+h*(z3_0);
    z1=(z1_0)+h*(z2_0);
    
    points[i]=(float*)malloc(sizeof(float)*2*n);
    points[i][0]=t;
    points[i][1]=z1;

    z1_0=z1;
    z2_0=z2;
    z3_0=z3;
    t+=h;
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
