#ifndef GRAPHICS_MODULE_H
#define GRAPHICS_MODULE_H

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

#include "math_module.h"

// custom pixel buffer (the canvas)
typedef struct pixel_buffer {
    int width;
    int height;
    uint32_t* color_buffer;
} pixel_buffer;

typedef struct triangle_face {
    int a;
    int b;
    int c;
} triangle_face;

typedef vector_3d vertex_3d;
typedef vector_2d vertex_2d;
typedef vector_2d_int pixel;

typedef struct model {
    vertex_3d* vertexes_3d;
    int num_vertexes;
    vertex_3d* vertexes_3d_transformed;
    vertex_2d* vertexes_2d;
    pixel* pixels;
    triangle_face* tri_faces;
    int num_faces;
} model;

//function pointer that accepts functions with a signature of receiving vector_3d and returning vector_2d
typedef vector_2d (*projection_function)(vector_3d);

//function pointer that ...
typedef void (*draw_triangle_function)(pixel_buffer*, float, float, float, float, float, float, uint32_t);

bool setup_pixel_buffer(pixel_buffer* p_buffer, int width, int height);

// clear the buffer with a single color
void clear_color_buffer(pixel_buffer* p_buffer, uint32_t color);

// draw a single point
void draw_pixel(pixel_buffer* p_buffer, int x, int y, uint32_t color);

// draw a single line using dda
void draw_line_dda(pixel_buffer* p_buffer, float x1, float y1, float x2, float y2, uint32_t color);

// draw a single line using bresenham
void draw_line_bresenham(pixel_buffer* p_buffer, int x1, int y1, int x2, int y2, uint32_t color);

vertex_2d viewport_transform(vertex_2d projected, float half_width, float half_height);

void draw_triangle_wireframe(pixel_buffer* p_buffer, float x1, float y1, float x2, float y2, float x3, float y3, uint32_t color);

void draw_horizontal_line(pixel_buffer* p_buffer, int x1, int x2, int y, uint32_t color);

void rasterize_triangle_scanline(pixel_buffer* p_buffer, float x1, float y1, float x2, float y2, float x3, float y3, uint32_t color);

void draw_model(pixel_buffer* p_buffer, model* md, projection_function project, draw_triangle_function draw, uint32_t color);

void draw_model_wireframe(pixel_buffer* p_buffer, model* md, projection_function project, uint32_t color);

void draw_model_scanlines(pixel_buffer* p_buffer, model* md, projection_function project, uint32_t color);

void scale_model(model* md, float factor_x, float factor_y, float factor_z);

void rotate_model(model* md, float angle_x, float angle_y, float angle_z);

void translate_model(model* md, float dx, float dy, float dz);

void reset_model_transform(model* md);

#endif // GRAPHICS_MODULE_H