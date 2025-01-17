#include <stdio.h>
#include <complex.h>
#include <math.h>
#include <stdlib.h>
#include "MatrixFunctions.h"
#include "GivensRotation.h"
#include "HouseholderReflection.h"
#include "Eigenvalue_calculator.h"
int main(){

    FILE* fptr=fopen("input.txt", "r");
    int n;
    /*
     *Input is being read from a text file
     *First line of the text file contains the order of the nxn matrix
     *Each line contains one element of input (Real, Complex)
     *Elements in the file are inputted into the matrix row-wise
    */
    fscanf(fptr, "%d", &n);
    double complex** matrix = zeros(n, n);
    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            double real, imag;
            fscanf(fptr, "%lf %lf", &real, &imag);
            matrix[i][j]=CMPLX(real, imag);
        }
    }
    fclose(fptr);
    double complex** eigenvalues=eigen(n, matrix);
 
    printf("Eigenvalues:\n");
    print_matrix(n, 1, eigenvalues);  
    return 0;
}


