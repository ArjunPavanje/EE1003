#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// Function to create, allocate memory for an mxn matrix
float **create_mat(int M, int N){ 
    float **matrix = malloc(sizeof(double *) * M);
    for (int i = 0; i < M; i++){
        matrix[i] = malloc(sizeof(float) * N);
    }
    return matrix;
}

// Function to print a matrix
void print_mat(float **A, int m, int n){
  for(int i=0; i<m; i++){
    for(int j=0; j<n; j++){
      printf("%lf ", A[i][j]);
    }
    printf("\n");
  }
}
// Function to solve a system of equations by LU Decomposition
float **LUsolver(int n){
  // Definining 'U' matrix
  float **U = create_mat(n, n);
  U[0][0] = 2;
  U[0][1] = 1;
  U[1][0] = 2;
  U[1][1] = -1;
  
  float **L = create_mat(n, n);
  
  //Initializing L, U as identity matrices,
  for(int i=0; i<n; i++){
    for(int j=0; j<n; j++){
      //U[i][j] = (i==j)?1:0;
      L[i][j] = (i==j)?1:0;    
    }
  }
  
  for(int i=0; i<(n-1); i++){
    // Checking if any pivot element is 0, in which case it is swapped with the next suitable row
    // If no such row is available, it is a singular matrix, can not be decomposed
    if(U[i][i]==0){
      int check=0;
      for(int j=i+1; j<n; j++){
        if(U[j][i] != 0){
          float *temp = U[i];
          U[i] = U[j];
          U[j] = temp;
          check = 1;
          break;
        }
      }
      if(check ==0){
      printf("Singular Matrix, cannot LU decompose");
      exit(-1);
      }
    }

    // Filling in values of L matrix which come out to be the coeffecients of row-reduction
    // Enteries of U matrix are obtained by row reduction
    for(int j=i+1; j<n; j++){
      float l_ij = U[j][i]/U[i][i];
      L[j][i] = l_ij;
      for(int k=i; k<n; k++){
        U[j][k] -= l_ij*U[i][k];
      }
    }
  }
  printf("U:\n");
  print_mat(U, n, n);
  printf("L:\n");
  print_mat(L, n, n);

  // Now that we have L, U; to solve the system of equations using them
  // Doing first backsubstitution to obtained Ly = b
  float **b = create_mat(n, 1);
  b[0][0] = 6;
  b[1][0] = 2;
  float **y = create_mat(n, 1);
  for(int i=0; i<n; i++){
    y[i][0] = b[i][0];
    for(int j=0; j<i; j++){
      y[i][0] -= L[i][j]*y[j][0];
    }
    y[i][0] /= L[i][i];
  }
  printf("y:\n");
  print_mat(y, 2, 1);
  
  float **x = create_mat(n, 1);
  // Now doing front-substitution to obtained Ux = y
  for(int i=n-1; i>=0; i--){
    x[i][0] = y[i][0];
    for(int j=n-1; j>i; j--){
      x[i][0] -= U[i][j]*x[j][0];
    }
    x[i][0] /= U[i][i];
  }
  printf("x:\n");
  print_mat(x, 2, 1);
  return x;
}


