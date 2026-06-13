#include "math_module.h"

vector_2d orthographic_projection(vector_3d v3d) {

    vector_2d v2d;

    v2d.x = v3d.x;
    v2d.y = v3d.y;

    return v2d;
}

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

vector_2d isometric_projection(vector_3d v3d) {

    // this projection is basically two rotations followed by an orthogonal projection

    float sqrt2 = 1.4142135623730950488016887242097f;
    float sqrt6 = 2.4494897427831780981972840747059f;

    vector_2d v2d;

    v2d.x = (v3d.x - v3d.z) / sqrt2;
    v2d.y = ((2*v3d.y) - v3d.x - v3d.z) / sqrt6;

    return v2d;
}

vector_2d simplified_perspective_projection(vector_3d v3d) {

    // assuming a particular camera position, camera orientation and screen position

    vector_2d v2d;

    v2d.x = v3d.x / v3d.z;
    v2d.y = v3d.y / v3d.z;

    return v2d;
}