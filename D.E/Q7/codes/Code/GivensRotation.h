#include <stdio.h>
#include <math.h>
#include <complex.h>
double complex** givens_rotation(int i, int j, int m, int n, double complex** matrix);
double complex** givens_rotation(int i, int j, int m, int n, double complex** matrix){
    double complex** G=eye(n);
    double r=sqrt(pow(cabs(matrix[i][i]),2) + pow(cabs(matrix[j][i]),2) );
    if(r < 1e-10){
        return matrix;
    }
    //conj is taking to account for cases when enteries of matrix are complex numbers
    double complex c=conj(matrix[i][i])/r;
    double complex s=conj(matrix[i+1][i])/r;
    //Accounting for case where either pivot or element to be nulled is 0
    if(cabs(matrix[i][i]) < 1e-10 ||  cabs(matrix[i+1][i]) < 1e-10){
        c=CMPLX(1,0);
        s=CMPLX(0,0);
    }
    G[i][i]=c;
    G[j][j]=conj(c);
    G[j][i]=-conj(s);
    G[i][j]=s;
    //Doing (G)(Matrix)(Gtranspose) to generate a similar matrix with an element nulled out
    double complex** Gtranspose=transpose(m, n, G);
    double complex** temp = matrix_multiply(n, n, n, G, matrix);
    matrix=matrix_multiply(n, n, n, temp, Gtranspose);
    return matrix; 
}
