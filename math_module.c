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