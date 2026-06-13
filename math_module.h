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

void scale_vector_3d(vector_3d* v3d, float factor_x, float factor_y, float factor_z);

void rotate_vector_3d(vector_3d* v3d, float angle_x, float angle_y, float angle_z);

void translate_vector_3d(vector_3d* v3d, float dx, float dy, float dz);

vector_2d isometric_projection(vector_3d v3d);

vector_2d simplified_perspective_projection(vector_3d v3d);

#endif // MATH_MODULE_H