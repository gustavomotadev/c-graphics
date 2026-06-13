#ifndef MATH_MODULE_H
#define MATH_MODULE_H

#include <math.h>

typedef struct vector_2d {
    float x;
    float y;
} vector_2d;

typedef struct vector_3d {
    float x;
    float y;
    float z;
} vector_3d;

typedef struct vector_2d_int {
    int x;
    int y;
} vector_2d_int;

vector_2d orthographic_projection(vector_3d v3d);

vector_2d_int round_vector_2d(vector_2d v2d);

#endif // MATH_MODULE_H