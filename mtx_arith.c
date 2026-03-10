#include "mtx.h"
#include <stdio.h>
#include <math.h>
#include <locale.h>
#include "mtx_struct.h"

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


int matrix_mul(matrix *m1, const matrix *m2) {
    if (!m1 || !m2) return -1;
    if (m1->w != m2->h) return -1; 
    size_t new_h = m1->h;
    size_t new_w = m2->w;


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