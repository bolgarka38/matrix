#ifndef MTX_H_INCLUDED
#define MTX_H_INCLUDED

#include <stddef.h>

typedef struct matrix matrix;


matrix *matrix_alloc(size_t w, size_t h);
matrix *matrix_copy(const matrix *m);

void matrix_free(matrix *m);
double *matrix_ptr(matrix *m, size_t i, size_t j);
const double *matrix_cptr(const matrix *m, size_t i, size_t j);
void matrix_set_zero(matrix *m);

matrix *matrix_set_id(matrix *m);
matrix *matrix_alloc_zero(size_t w, size_t h);
matrix *matrix_alloc_id(size_t w);
matrix *matrix_assign(matrix *m1, const matrix *m2);

void matrix_print(const matrix *m);

matrix *matrix_scan(void);

int matrix_add(matrix *m1, const matrix *m2);
int matrix_sub(matrix *m1, const matrix *m2);
void matrix_smul(matrix *m, double d);
void matrix_sdiv(matrix *m, double d);
int matrix_add2(matrix *m, const matrix *m1, const matrix *m2);
int matrix_sub2(matrix *m, const matrix *m1, const matrix *m2);
int matrix_smul2(matrix *m, const matrix *m1, double d);
int matrix_sdiv2(matrix *m, const matrix *m1, double d);
int matrix_mul(matrix *m1, const matrix *m2);
int matrix_mul2(matrix *m, const matrix *m1, const matrix *m2);




int matrix_transpose(matrix *m);
int matrix_swap_rows(matrix *m, size_t r1, size_t r2);
int matrix_swap_cols(matrix *m, size_t c1, size_t c2);
int matrix_row_mul(matrix *m, size_t r, double d);
int matrix_row_div(matrix *m, size_t r, double d);
int matrix_row_add(matrix *m, size_t r1, size_t r2, double coeff);
double matrix_norm(const matrix *m);




matrix *matrix_exp(const matrix *A, double eps);
matrix *matrix_solve(const matrix *A, const matrix *B);


#endif // MTX_H_INCLUDED
