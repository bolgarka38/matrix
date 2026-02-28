#include "mtx.h"
#include <stdio.h>
#include <math.h>
#include <locale.h>

int main() {
    setlocale(LC_ALL, "Russian");
    /* ���� ��������� ���������� */
    printf("=== ���� ��������� ���������� ===\n");
    matrix *A = matrix_alloc(3, 3);
    *matrix_ptr(A,0,0) = 1; *matrix_ptr(A,0,1) = 0; *matrix_ptr(A,0,2) = 0;
    *matrix_ptr(A,1,0) = 0; *matrix_ptr(A,1,1) = 2; *matrix_ptr(A,1,2) = 0;
    *matrix_ptr(A,2,0) = 0; *matrix_ptr(A,2,1) = 0; *matrix_ptr(A,2,2) = -1;
    printf("A =\n");
    matrix_print(A);

    matrix *expA = matrix_exp(A, 1e-10);
    printf("exp(A) � ��������� 1e-10:\n");
    matrix_print(expA);
    /* ������� ������������ ������� � ���������� e, e^2, 1/e */
    matrix_free(expA);
    matrix_free(A);

    /* ���� ������� ���� */
    printf("\n=== ���� ������� ���� ===\n");
    /* �������:
     * 2x + y = 5
     * x - y = 1
     * �������: x=2, y=1 */
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
        printf("������� X =\n");
        matrix_print(X2);

        /* ��������: �������� A*X - B � ����� ������� */
        matrix *AX = matrix_alloc(1,2);
        matrix_mul2(AX, A2, X2);
        matrix_sub(AX, B2);
        printf("���������� ||A X - B|| = %f\n", matrix_norm(AX));
        matrix_free(AX);
        matrix_free(X2);
    } else printf("������� ���������!\n");

    matrix_free(A2);
    matrix_free(B2);

    return 0;
}
