#include <stdio.h>

#include <math.h>
#include <complex.h>

double complex** matscale(int m, int n, double complex** matrix, double k);//Scaling a matrix by a constant 'k'
double complex** matrix_multiply(int m, int p, int n, double complex** A, double complex** B);//Multiplying 2 matrices
double complex** transpose(int m, int n, double complex** matrix);//Taking conjugate transpose of a matrix
void print_matrix(int m, int n, double complex** matrix);//Printing a matrix
int isUpperTriangular(int n, double complex** matrix);//Checking if a matrix is upper triangular
double complex** eye(int n);//generating an nxn identity matrix
double complex** zeros(int m, int n);//enters a matrix with all zeros                           

double complex** zeros(int m, int n){
    double complex** matrix = (double complex**) malloc(sizeof(double complex)*m);
    for(int i = 0; i < m; i++){
        matrix[i] = (double complex*) malloc(sizeof(double complex)*n);
        for(int j = 0; j < n; j++){
            matrix[i][j] = CMPLX(0,0);
        }
    }
    return matrix;
}

double complex** matscale(int m, int n, double complex** matrix, double k){
    double complex** ret = zeros(m, n);
    for(int i=0;i<m;i++){
        for(int j=0;j<n;j++){
            ret[i][j]=k*matrix[i][j];
        }
    }
    return ret;
}
double complex** matrix_multiply(int m, int p, int n, double complex** A, double complex** B){
    double complex** C=zeros(m, n);
    for (int i=0;i<m;i++) {
        for (int j=0;j<n;j++) {
            C[i][j] = 0;
            for (int k=0;k<p;k++) {
                C[i][j]+= A[i][k]*B[k][j];
            }
        }
    }
    return C;
}
double complex** transpose(int m, int n, double complex** matrix){
    double complex** transpose=zeros(n, m);
    for(int i=0;i<m;i++){
        for(int j=0;j<n;j++){
            transpose[j][i]=conj(matrix[i][j]);
        }
    }
    return transpose;
}
void print_matrix(int m, int n, double complex** matrix){
    for(int i=0; i<m; i++){
        for(int j=0; j<n; j++){
            printf("(%lf + %lfi) ", creal(matrix[i][j]), cimag(matrix[i][j]));
        }
        printf("\n");
    }
}
double complex** eye(int n){
    double complex** eye=(double complex**)malloc(sizeof(double complex)*n);
    for(int i=0; i<n; i++){
        eye[i]=(double complex*)malloc(sizeof(double complex)*n);
        for(int j=0; j<n; j++){
            eye[i][j]=(i==j)?CMPLX(1,0):CMPLX(0,0);
        }
    }
    return eye;
}
int isUpperTriangular(int n, double complex** matrix){
    double tolerance=1e-6;
    for(int i=0; i<n-1; i++){
        if(cabs(matrix[i+1][i]) > tolerance) return 0;
    }
    return 1;
}
