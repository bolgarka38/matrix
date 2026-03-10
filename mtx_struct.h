#ifndef MTX_STRUCT_H_INCLUDED
#define MTX_STRUCT_H_INCLUDED

#include <stddef.h>

struct matrix {
    double *data;
    size_t w;
    size_t h;
};

matrix *matrix_alloc_common(size_t w, size_t h);

#endif