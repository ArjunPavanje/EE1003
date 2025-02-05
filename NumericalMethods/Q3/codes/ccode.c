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

float **pointsGet(float h, float y, float x, int n){ //taking initial values of 'x', step size, number of points as input

  float **points = (float **) malloc(sizeof(float *)*2*n);
  for (int i=0; i<2*n; i++){
    points[i]=(float*)malloc(sizeof(float)*2);

    points[i][0]=x;
    points[i][1]=y;
    
    y=y-h*((y*y + y + 1)/(x*x + x +1));
    x+=h; //Iteratively increasing 'x' value
  }
  return points;
}

// As both simulated and theoretical plot are symmetric, only one side (+x axis) side is calculated.
//To free up used memory
void free_memory(float **points, int n){
  for(int i=0;i<2*n; i++){
    free(points[i]);
  }
  free(points);
}
