#include "mtx.h"
#include <stdio.h>
#include <math.h>
#include <locale.h>

int main() {
    printf("Matrix exponential test\n");
    matrix *A = matrix_alloc(3, 3);
    *matrix_ptr(A,0,0) = 1; *matrix_ptr(A,0,1) = 0; *matrix_ptr(A,0,2) = 0;
    *matrix_ptr(A,1,0) = 0; *matrix_ptr(A,1,1) = 2; *matrix_ptr(A,1,2) = 0;
    *matrix_ptr(A,2,0) = 0; *matrix_ptr(A,2,1) = 0; *matrix_ptr(A,2,2) = -1;
    printf("A =\n");
    matrix_print(A);

    matrix *expA = matrix_exp(A, 1e-10);
    printf("exp(A) accurate to 1e-10:\n");
    matrix_print(expA);
  
    matrix_free(expA);
    matrix_free(A);

    printf("\n Linear system test\n");

    matrix *A2 = matrix_alloc(2,2);
    *matrix_ptr(A2,0,0)=2; *matrix_ptr(A2,0,1)=1;
    *matrix_ptr(A2,1,0)=1; *matrix_ptr(A2,1,1)=-1;

    matrix *B2 = matrix_alloc(1,2);
    *matrix_ptr(B2,0,0)=5;
    *matrix_ptr(B2,1,0)=1;

    printf("A =\n");
    matrix_print(A2);
    printf("B =\n");
    matrix_print(B2);

    matrix *X2 = matrix_solve(A2, B2);
    if (X2) {
        printf("Answer X =\n");
        matrix_print(X2);

     
        matrix *AX = matrix_alloc(1,2);
        matrix_mul2(AX, A2, X2);
        matrix_sub(AX, B2);
        printf("Residual ||A X - B|| = %f\n", matrix_norm(AX));
        matrix_free(AX);
        matrix_free(X2);
    } else printf("Singular linear system!\n");

    matrix_free(A2);
    matrix_free(B2);



    printf("\nSecond test\n");
    matrix *A3 = matrix_alloc(3, 3);
    *matrix_ptr(A3,0,0) = 0; *matrix_ptr(A3,0,1) = 0; *matrix_ptr(A3,0,2) = 1;
    *matrix_ptr(A3,1,0) = 0; *matrix_ptr(A3,1,1) = 1; *matrix_ptr(A3,1,2) = 0;
    *matrix_ptr(A3,2,0) = 1; *matrix_ptr(A3,2,1) = 0; *matrix_ptr(A3,2,2) = 0;

    matrix *B3 = matrix_alloc(1, 3);
    *matrix_ptr(B3,0,0) = 1;
    *matrix_ptr(B3,1,0) = 2;
    *matrix_ptr(B3,2,0) = 3;

    printf("A =\n");
    matrix_print(A3);
    printf("B =\n");
    matrix_print(B3);

    matrix *X3 = matrix_solve(A3, B3);
    if (X3) {
        printf("Answer X =\n");
        matrix_print(X3);
        matrix *AX3 = matrix_alloc(1, 3);
        matrix_mul2(AX3, A3, X3);
        matrix_sub(AX3, B3);
        printf("Residual ||A X - B|| = %g\n", matrix_norm(AX3));
        matrix_free(AX3);
        matrix_free(X3);
    } else {
        printf("Singular linear system!\n");
    }

    matrix_free(A3);
    matrix_free(B3);

    return 0;
}
