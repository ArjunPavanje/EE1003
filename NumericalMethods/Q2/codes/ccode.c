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

float **pointsGet(float h, float x, int n){ //taking initial values of 'x', step size, number of points as input
  float y=log(fabs(sin(x)))/(1+x*x); //initial value of function

  float **points = (float **) malloc(sizeof(float *)*2*n);
  for (int i=0; i<n; i++){
    points[i]=(float*)malloc(sizeof(float)*2);
    points[n+i]=(float*)malloc(sizeof(float)*2);

    points[i][0]=x;
    points[n+i][0]=-x;

    if(i>=2 && fabs(y-points[i-1][1])>=0.3){ //Accounting for discontinuity of f'(x) at points
      y=points[i-1][1];  
    }
    points[i][1]=y;
    points[n+i][1]=y;

    y+=h*(  (  (cos(x)/sin(x))  -  (2*x*y)  )  /  (1+x*x)  );
    x+=h; //Iteratively increasing 'x' value
  }
  return points;
}


float **theoreticalPoints(float h, int n){ //taking step size, number of points to plot as input
  
  float **points = (float **) malloc(sizeof(float *)*2*n);
  float x=0.1;
  for(int i=0; i<n; i++){
    float y = log(fabs(sin(x)))/(1+x*x);
    points[i]=(float*)malloc(sizeof(float)*2);
    points[n+i]=(float*)malloc(sizeof(float)*2);
    points[i][0]=x;
    points[i][1]=y;
    points[n+i][0]=-x;
    points[n+i][1]=y;
    x=x+h;
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
