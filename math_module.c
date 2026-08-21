#include "math_module.h"

vector_2d_int round_vector_2d(vector_2d v2d) {

    vector_2d_int v2di;

    v2di.x = (int) roundf(v2d.x);
    v2di.y = (int) roundf(v2d.y);

    return v2di;
}

void scale_vector_3d(vector_3d* v3d, float factor_x, float factor_y, float factor_z) {

    v3d->x *= factor_x;
    v3d->y *= factor_y;
    v3d->z *= factor_z;
}

void rotate_vector_3d(vector_3d* v3d, float angle_x, float angle_y, float angle_z) {

    // Cache the initial values
    float x = v3d->x;
    float y = v3d->y;
    float z = v3d->z;

    // 1. Rotate around X-axis
    float cos_x = cosf(angle_x);
    float sin_x = sinf(angle_x);
    float y1 = y * cos_x - z * sin_x;
    float z1 = y * sin_x + z * cos_x;

    // 2. Rotate around Y-axis
    // Note: uses the original x, but the updated z1
    float cos_y = cosf(angle_y);
    float sin_y = sinf(angle_y);
    float x2 = x * cos_y + z1 * sin_y;
    float z2 = -x * sin_y + z1 * cos_y;

    // 3. Rotate around Z-axis
    // Note: uses the updated x2 and y1
    float cos_z = cosf(angle_z);
    float sin_z = sinf(angle_z);
    float x3 = x2 * cos_z - y1 * sin_z;
    float y3 = x2 * sin_z + y1 * cos_z;

    // Apply final coordinates to the struct
    v3d->x = x3;
    v3d->y = y3;
    v3d->z = z2;
}

void translate_vector_3d(vector_3d* v3d, float dx, float dy, float dz) {

    v3d->x += dx;
    v3d->y += dy;
    v3d->z += dz;
}

bool compare_floats(float a, float b, float epsilon) {

    return fabsf(a - b) < epsilon ? true : false;
}

bool lesser_equal_floats(float first, float second, float epsilon) {

    if (first < second) {
        return true;
    }

    if (fabsf(first - second) < epsilon) {
        return true;
    }

    return false;
}

vector_3d subtract_vector_3d(vector_3d first, vector_3d second) {
    vector_3d result;

    result.x = first.x - second.x;
    result.y = first.y - second.y;
    result.z = first.z - second.z;

    return result;
}

vector_3d cross_product_vector_3d(vector_3d first, vector_3d second) {
    vector_3d result;

    result.x = (first.y * second.z) - (first.z * second.y);
    result.y = (first.z * second.x) - (first.x * second.z);
    result.z = (first.x * second.y) - (first.y * second.x);

    return result;
}

float euclidean_norm_vector_3d(vector_3d v3d) {
    
    return sqrtf((v3d.x*v3d.x) + (v3d.y*v3d.y) + (v3d.z*v3d.z));
}

vector_3d zero_vector_3d() {

    vector_3d zero;
    zero.x = 0.0f;
    zero.y = 0.0f;
    zero.z = 0.0f;

    return zero;
}

vector_3d normalize_vector_3d(vector_3d v3d) {

    float euclidean_norm = euclidean_norm_vector_3d(v3d);

    if (compare_floats(euclidean_norm, 0.0f, DEFAULT_EPSILON)) {

        return zero_vector_3d();
    }

    v3d.x /= euclidean_norm;
    v3d.y /= euclidean_norm;
    v3d.z /= euclidean_norm;

    return v3d;
}

vector_3d compute_triangle_normal(vector_3d p1, vector_3d p2, vector_3d p3) {

    vector_3d edge_a = subtract_vector_3d(p2, p1);
    vector_3d edge_b = subtract_vector_3d(p3, p1);

    vector_3d normal = cross_product_vector_3d(edge_a, edge_b);

    return normalize_vector_3d(normal);
}

float dot_product_vector_3d(vector_3d first, vector_3d second) {

    return (first.x*second.x) + (first.y*second.y) + (first.z*second.z);
}

vector_3d compute_triangle_centroid(vector_3d p1, vector_3d p2, vector_3d p3) {
    
    vector_3d centroid;

    centroid.x = (p1.x + p2.x + p3.x)/3;
    centroid.y = (p1.y + p2.y + p3.y)/3;
    centroid.z = (p1.z + p2.z + p3.z)/3;

    return centroid;
}

float clampf(float val, float min, float max) {
    return val < min ? min : (val > max ? max : val);
}

int clampi(int val, int min, int max) {
    return val < min ? min : (val > max ? max : val);
}

float min3f(float a, float b, float c) {
    return (a < b) ? ((a < c) ? a : c) : ((b < c) ? b : c);
}

float max3f(float a, float b, float c) {
    return (a > b) ? ((a > c) ? a : c) : ((b > c) ? b : c);
}

void minmax3f(float a, float b, float c, float *min, float *max) {
    if (a > b) {
        *max = (a > c) ? a : c;
        *min = (b < c) ? b : c;
    } else {
        *max = (b > c) ? b : c;
        *min = (a < c) ? a : c;
    }
}

matrix_4d multiply_matrix_4d(matrix_4d left, matrix_4d right) {

    matrix_4d result;

    for (int c = 0; c < 4; c++)
    {
        for (int l = 0; l < 4; l++)
        {
            // column c line l
            result.m_cl[c][l] = left.m_cl[0][l]*right.m_cl[c][0] + left.m_cl[1][l]*right.m_cl[c][1] + left.m_cl[2][l]*right.m_cl[c][2] + left.m_cl[3][l]*right.m_cl[c][3];
        }
        
    }

    return result;
}

vector_4d multiply_matrix_vector_4d(matrix_4d m4d, vector_4d v4d) {

    vector_4d result;

    result.x = m4d.m_cl[0][0]*v4d.x + m4d.m_cl[1][0]*v4d.y + m4d.m_cl[2][0]*v4d.z + m4d.m_cl[3][0]*v4d.w;
    result.y = m4d.m_cl[0][1]*v4d.x + m4d.m_cl[1][1]*v4d.y + m4d.m_cl[2][1]*v4d.z + m4d.m_cl[3][1]*v4d.w;
    result.z = m4d.m_cl[0][2]*v4d.x + m4d.m_cl[1][2]*v4d.y + m4d.m_cl[2][2]*v4d.z + m4d.m_cl[3][2]*v4d.w;
    result.w = m4d.m_cl[0][3]*v4d.x + m4d.m_cl[1][3]*v4d.y + m4d.m_cl[2][3]*v4d.z + m4d.m_cl[3][3]*v4d.w;

    return result;
}

void print_matrix_4d(matrix_4d m4d) {

    for (int l = 0; l < 4; l++)
    {
        printf("| ");
        for (int c = 0; c < 4; c++)
        {
            printf("%5.1f ", m4d.m_cl[c][l]);
        }
        printf("|\n");
    }
}