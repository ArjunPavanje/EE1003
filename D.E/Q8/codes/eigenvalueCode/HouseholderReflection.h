#include <stdio.h>
#include <math.h>
#include <complex.h>
double complex** householder(int n,double complex** matrix);
double complex** householder(int n, double complex** matrix){
    //'i' is mumn no. 'i' goes till n-2 because in upper hessenberg form last two ls remain unchanged.
    for(int i=0;i<n-1;i++){
        double complex** x=zeros(n-i-1, 1);
        int count=0;
        for(int j=i+1;j<n;j++){
            x[count][0]=matrix[j][i];
            count++;
        }
        //'x' is the 'i+1'th column of matrix
        
        double norm_x=0;
        for(int j=0;j<count;j++){
            norm_x+=pow(cabs(x[j][0]),2);
        }
        norm_x=sqrt(norm_x);
        if(x[0][0]==0){
            x[0][0]=-norm_x;
        }
        else{
            x[0][0]+=(CMPLX((norm_x*cos(carg(x[0][0]))) , (norm_x*sin(carg(x[0][0]))) ));
        }

        //turning 'x' into 'u' the unit householder reflector vector
        norm_x=0;
        for(int j=0;j<count;j++){
            norm_x+=pow(cabs(x[j][0]),2);
        }
        norm_x=sqrt(norm_x);
        if(norm_x > 1e-10){ //Accounting for cases like null matrix
            x=matscale(count, 1, x, 1/norm_x);
        }
    
        double complex** xT = transpose(n-i-1, 1, x);

        //generating  householder reflector matrix 'P'
        double complex** P=eye(n-i-1);

        double complex**  u_outer_product=matrix_multiply(n-i-1, 1, n-i-1, x, xT);
        for (int l = 0; l < n-i-1; l++) {
            for (int m = 0; m < n-i-1; m++) {
                P[l][m] -= 2 * u_outer_product[l][m];
            }
        }
        
        double complex** H=zeros(n, n);
        for(int j=0;j<n;j++){//'j'=row
            for(int k=0;k<n;k++){//'k'=columns
                
                if(k>(i) && j>(i)){
                    H[j][k]=P[j-i-1][k-i-1];
                }
                else{
                    H[j][k]= (j==k)?CMPLX(1,0):CMPLX(0,0);                
                }
            }
        }
       //Applying Householder reflection to matrix
        double complex** HT=transpose(n, n, H);
        double complex** result=matrix_multiply(n, n, n, matrix, H);
        matrix=matrix_multiply(n, n, n, H, result);
    }
    return matrix;
}
