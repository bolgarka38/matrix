#include "mtx.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/* Полное определение структуры (инкапсуляция) */
struct matrix {
    double *data;  //линейный массив элементов, хранящийся по строкам
    size_t w;  //число столбцов
    size_t h;  //число строк
};


/* Вспомогательная: выделение памяти под структуру и массив данных */
static matrix *alloc_common(size_t w, size_t h) {
    if (w == 0 || h == 0) return NULL;
    matrix *m = malloc(sizeof(matrix));
    if (!m) return NULL;
    m->data = malloc(w * h * sizeof(double));
    if (!m->data) {
        free(m);
        return NULL;
    }
    m->w = w;
    m->h = h;
    return m;
}


matrix *matrix_alloc(size_t w, size_t h) {
    return alloc_common(w, h);
}


void matrix_free(matrix *m) {
    if (m) {
        free(m->data);
        free(m);
    }
}


matrix *matrix_copy(const matrix *m) {
    if (!m) return NULL;
    matrix *c = matrix_alloc(m->w, m->h);
    if (!c) return NULL;
    memcpy(c->data, m->data, m->w * m->h * sizeof(double));
    return c;
}


double *matrix_ptr(matrix *m, size_t i, size_t j) {
    return &m->data[i * m->w + j];
}


const double *matrix_cptr(const matrix *m, size_t i, size_t j) {
    return &m->data[i * m->w + j];
}


void matrix_set_zero(matrix *m) {
    if (!m) return;
    memset(m->data, 0, m->w * m->h * sizeof(double));
}


matrix *matrix_set_id(matrix *m) {
    if (!m) return NULL;
    matrix_set_zero(m);
    size_t d = (m->w < m->h) ? m->w : m->h;
    for (size_t i = 0; i < d; ++i)
        *matrix_ptr(m, i, i) = 1.0;
    return m;
}


matrix *matrix_alloc_zero(size_t w, size_t h) {
    matrix *m = matrix_alloc(w, h);
    if (m) matrix_set_zero(m);
    return m;
}


matrix *matrix_alloc_id(size_t w) {
    matrix *m = matrix_alloc(w, w);
    if (m) matrix_set_id(m);
    return m;
}


matrix *matrix_assign(matrix *m1, const matrix *m2) {
    if (!m1 || !m2) return NULL;
    if (m1->w != m2->w || m1->h != m2->h) return NULL;
    memcpy(m1->data, m2->data, m1->w * m1->h * sizeof(double));
    return m1;
}


void matrix_print(const matrix *m) {
    if (!m) return;
    printf("Matrix %zux%zu:\n", m->w, m->h);
    for (size_t i = 0; i < m->h; ++i) {
        for (size_t j = 0; j < m->w; ++j) {
            printf("%8.3f ", *matrix_cptr(m, i, j));
        }
        printf("\n");
    }
}


matrix *matrix_scan(void) {
    size_t w, h;
    printf("Enter width and height: ");
    if (scanf("%zu %zu", &w, &h) != 2) return NULL;
    matrix *m = matrix_alloc(w, h);
    if (!m) return NULL;
    printf("Enter %zu elements row by row:\n", w * h);
    for (size_t i = 0; i < h; ++i) {
        for (size_t j = 0; j < w; ++j) {
            if (scanf("%lf", matrix_ptr(m, i, j)) != 1) {
                matrix_free(m);
                return NULL;
            }
        }
    }
    return m;
}



/* Арифметические действия */
int matrix_add(matrix *m1, const matrix *m2) {
    if (!m1 || !m2) return -1;
    if (m1->w != m2->w || m1->h != m2->h) return -1;
    size_t n = m1->w * m1->h;
    for (size_t i = 0; i < n; ++i)
        m1->data[i] += m2->data[i];
    return 0;
}


int matrix_sub(matrix *m1, const matrix *m2) {
    if (!m1 || !m2) return -1;
    if (m1->w != m2->w || m1->h != m2->h) return -1;
    size_t n = m1->w * m1->h;
    for (size_t i = 0; i < n; ++i)
        m1->data[i] -= m2->data[i];
    return 0;
}


void matrix_smul(matrix *m, double d) {
    if (!m) return;
    size_t n = m->w * m->h;
    for (size_t i = 0; i < n; ++i)
        m->data[i] *= d;
}


void matrix_sdiv(matrix *m, double d) {
    if (!m || d == 0.0) return;
    matrix_smul(m, 1.0 / d);
}


int matrix_add2(matrix *m, const matrix *m1, const matrix *m2) {
    if (!m || !m1 || !m2) return -1;
    if (m1->w != m2->w || m1->h != m2->h) return -1;
    if (m->w != m1->w || m->h != m1->h) return -1;
    size_t n = m->w * m->h;
    for (size_t i = 0; i < n; ++i)
        m->data[i] = m1->data[i] + m2->data[i];
    return 0;
}


int matrix_sub2(matrix *m, const matrix *m1, const matrix *m2) {
    if (!m || !m1 || !m2) return -1;
    if (m1->w != m2->w || m1->h != m2->h) return -1;
    if (m->w != m1->w || m->h != m1->h) return -1;
    size_t n = m->w * m->h;
    for (size_t i = 0; i < n; ++i)
        m->data[i] = m1->data[i] - m2->data[i];
    return 0;
}


int matrix_smul2(matrix *m, const matrix *m1, double d) {
    if (!m || !m1) return -1;
    if (m->w != m1->w || m->h != m1->h) return -1;
    size_t n = m->w * m->h;
    for (size_t i = 0; i < n; ++i)
        m->data[i] = m1->data[i] * d;
    return 0;
}


int matrix_sdiv2(matrix *m, const matrix *m1, double d) {
    if (d == 0.0) return -1;
    return matrix_smul2(m, m1, 1.0 / d);
}

/* Умножение матриц */
int matrix_mul(matrix *m1, const matrix *m2) {
    if (!m1 || !m2) return -1;
    if (m1->w != m2->h) return -1;      /* внутренние размерности */
    size_t new_h = m1->h;
    size_t new_w = m2->w;

    /* Выделяем временную матрицу для результата */
    matrix *tmp = matrix_alloc(new_w, new_h);
    if (!tmp) return -1;

    for (size_t i = 0; i < new_h; ++i) {
        for (size_t j = 0; j < new_w; ++j) {
            double s = 0.0;
            for (size_t k = 0; k < m1->w; ++k)
                s += *matrix_cptr(m1, i, k) * *matrix_cptr(m2, k, j);
            *matrix_ptr(tmp, i, j) = s;
        }
    }

    /* Переносим результат в m1, изменяя размер при необходимости */
    double *new_data = malloc(new_h * new_w * sizeof(double));
    if (!new_data) {
        matrix_free(tmp);
        return -1;
    }
    memcpy(new_data, tmp->data, new_h * new_w * sizeof(double));
    free(m1->data);
    m1->data = new_data;
    m1->w = new_w;
    m1->h = new_h;

    matrix_free(tmp);
    return 0;
}



int matrix_mul2(matrix *m, const matrix *m1, const matrix *m2) {
    if (!m || !m1 || !m2) return -1;
    if (m1->w != m2->h) return -1;
    size_t new_h = m1->h;
    size_t new_w = m2->w;

    /* Временная матрица для результата */
    matrix *tmp = matrix_alloc(new_w, new_h);
    if (!tmp) return -1;

    for (size_t i = 0; i < new_h; ++i) {
        for (size_t j = 0; j < new_w; ++j) {
            double s = 0.0;
            for (size_t k = 0; k < m1->w; ++k)
                s += *matrix_cptr(m1, i, k) * *matrix_cptr(m2, k, j);
            *matrix_ptr(tmp, i, j) = s;
        }
    }

    /* Если m имеет неподходящий размер, перевыделяем память */
    if (m->w != new_w || m->h != new_h) {
        double *new_data = malloc(new_h * new_w * sizeof(double));
        if (!new_data) {
            matrix_free(tmp);
            return -1;
        }
        free(m->data);
        m->data = new_data;
        m->w = new_w;
        m->h = new_h;
    }
    memcpy(m->data, tmp->data, new_h * new_w * sizeof(double));
    matrix_free(tmp);
    return 0;
}




/* Манипуляции */
int matrix_transpose(matrix *m) {
    if (!m || m->w != m->h) return -1;
    size_t n = m->w;
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = i + 1; j < n; ++j) {
            double *a = matrix_ptr(m, i, j);
            double *b = matrix_ptr(m, j, i);
            double t = *a;
            *a = *b;
            *b = t;
        }
    }
    return 0;
}


int matrix_swap_rows(matrix *m, size_t r1, size_t r2) {
    if (!m || r1 >= m->h || r2 >= m->h) return -1;
    if (r1 == r2) return 0;
    double *row1 = m->data + r1 * m->w;
    double *row2 = m->data + r2 * m->w;
    for (size_t j = 0; j < m->w; ++j) {
        double t = row1[j];
        row1[j] = row2[j];
        row2[j] = t;
    }
    return 0;
}


int matrix_swap_cols(matrix *m, size_t c1, size_t c2) {
    if (!m || c1 >= m->w || c2 >= m->w) return -1;
    if (c1 == c2) return 0;
    for (size_t i = 0; i < m->h; ++i) {
        double *row = m->data + i * m->w;
        double t = row[c1];
        row[c1] = row[c2];
        row[c2] = t;
    }
    return 0;
}


int matrix_row_mul(matrix *m, size_t r, double d) {
    if (!m || r >= m->h) return -1;
    double *row = m->data + r * m->w;
    for (size_t j = 0; j < m->w; ++j)
        row[j] *= d;
    return 0;
}


int matrix_row_div(matrix *m, size_t r, double d) {
    if (d == 0.0) return -1;
    return matrix_row_mul(m, r, 1.0 / d);
}


int matrix_row_add(matrix *m, size_t r1, size_t r2, double coeff) {
    if (!m || r1 >= m->h || r2 >= m->h) return -1;
    double *row1 = m->data + r1 * m->w;
    double *row2 = m->data + r2 * m->w;
    for (size_t j = 0; j < m->w; ++j)
        row1[j] += coeff * row2[j];
    return 0;
}


double matrix_norm(const matrix *m) {
    if (!m) return 0.0;
    double max = 0.0;
    for (size_t i = 0; i < m->h; ++i) {
        double sum = 0.0;
        for (size_t j = 0; j < m->w; ++j)
            sum += fabs(*matrix_cptr(m, i, j));
        if (sum > max) max = sum;
    }
    return max;
}



/* Матричная экспонента */
matrix *matrix_exp(const matrix *A, double eps) {
    if (!A || A->w != A->h) return NULL;
    size_t n = A->w;

    matrix *result = matrix_alloc_id(n);
    if (!result) return NULL;

    matrix *term = matrix_copy(A);   //A^1/1!
    if (!term) {
        matrix_free(result);
        return NULL;
    }

    matrix *next = NULL;
    unsigned long k = 1;   // term соответствует A^k/k!
    double norm_term = matrix_norm(term);

    while (norm_term >= eps) {
        /* Добавляем текущий член к результату */
        matrix_add(result, term);

        /* Вычисляем следующий член: term * A / (k+1) */
        next = matrix_alloc(n, n);
        if (!next) {
            matrix_free(result);
            matrix_free(term);
            return NULL;
        }
        if (matrix_mul2(next, term, A) != 0) {
            matrix_free(next);
            matrix_free(result);
            matrix_free(term);
            return NULL;
        }
        matrix_sdiv(next, (double)(k + 1));

        matrix_free(term);
        term = next;
        norm_term = matrix_norm(term);
        ++k;
    }

    matrix_free(term);
    return result;
}



/* Решение СЛАУ методом Гаусса с выбором главного элемента */
matrix *matrix_solve(const matrix *A, const matrix *B) {
    if (!A || !B || A->w != A->h) return NULL;
    size_t n = A->w;
    if (B->h != n) return NULL;
    size_t m = B->w;   //число правых частей

    /* Строим расширенную матрицу [A | B] размера n x (n+m) */
    matrix *aug = matrix_alloc(n + m, n);
    if (!aug) return NULL;

    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < n; ++j)
            *matrix_ptr(aug, i, j) = *matrix_cptr(A, i, j);
        for (size_t j = 0; j < m; ++j)
            *matrix_ptr(aug, i, n + j) = *matrix_cptr(B, i, j);
    }

    /* Прямой ход с выбором главного элемента по столбцу */
    for (size_t k = 0; k < n; ++k) {
        /* Поиск максимального элемента в столбце k начиная с k-й строки */
        size_t pivot = k;
        double max_val = fabs(*matrix_cptr(aug, k, k));
        for (size_t i = k + 1; i < n; ++i) {
            double val = fabs(*matrix_cptr(aug, i, k));
            if (val > max_val) {
                max_val = val;
                pivot = i;
            }
        }
        if (max_val < 1e-15) {   //матрица вырождена
            matrix_free(aug);
            return NULL;
        }
        if (pivot != k)
            matrix_swap_rows(aug, k, pivot);

        /* Нормализация k-й строки */
        double pivot_val = *matrix_cptr(aug, k, k);
        matrix_row_div(aug, k, pivot_val);

        /* Обнуление элементов ниже диагонали */
        for (size_t i = k + 1; i < n; ++i) {
            double factor = *matrix_cptr(aug, i, k);
            if (fabs(factor) > 1e-15)
                matrix_row_add(aug, i, k, -factor);
        }
    }

    /* Обратный ход (обнуление над диагональю) */
    for (size_t k = n; k-- > 0; ) {
        for (size_t i = k; i-- > 0; ) {
            double factor = *matrix_cptr(aug, i, k);
            if (fabs(factor) > 1e-15)
                matrix_row_add(aug, i, k, -factor);
        }
    }

    /* Извлечение решения X из последних m столбцов */
    matrix *X = matrix_alloc(m, n);
    if (!X) {
        matrix_free(aug);
        return NULL;
    }
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < m; ++j)
            *matrix_ptr(X, i, j) = *matrix_cptr(aug, i, n + j);
    }

    matrix_free(aug);
    return X;
}
