#include "mtx.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "mtx_struct.h"


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
