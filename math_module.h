#ifndef MATH_MODULE_H
#define MATH_MODULE_H

#include <math.h>
#include <stdbool.h>

#define DEFAULT_EPSILON 0.0001f

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

vector_2d_int round_vector_2d(vector_2d v2d);

void scale_vector_3d(vector_3d* v3d, float factor_x, float factor_y, float factor_z);

void rotate_vector_3d(vector_3d* v3d, float angle_x, float angle_y, float angle_z);

void translate_vector_3d(vector_3d* v3d, float dx, float dy, float dz);

bool compare_floats(float a, float b, float epsilon);

bool lesser_equal_floats(float first, float second, float epsilon);

vector_3d subtract_vector_3d(vector_3d first, vector_3d second);

vector_3d cross_product_vector_3d(vector_3d first, vector_3d second);

float euclidean_norm_vector_3d(vector_3d v3d);

vector_3d zero_vector_3d();

vector_3d normalize_vector_3d(vector_3d v3d);

vector_3d compute_triangle_normal(vector_3d p1, vector_3d p2, vector_3d p3);

float dot_product_vector_3d(vector_3d first, vector_3d second);

vector_3d compute_triangle_centroid(vector_3d p1, vector_3d p2, vector_3d p3);

float clampf(float val, float min, float max);

int clampi(int val, int min, int max);

float min3f(float a, float b, float c);

float max3f(float a, float b, float c);

void minmax3f(float a, float b, float c, float *min, float *max);

#endif // MATH_MODULE_H