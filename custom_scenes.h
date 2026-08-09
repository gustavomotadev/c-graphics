#ifndef SCENES_MODULE_H
#define SCENES_MODULE_H

#include <stdlib.h>
#include <time.h>

#include "graphics_module.h"

void checkered_board_animation(pixel_buffer* p_buffer, double delta_time, double total_time);

void lines_dda_triangle(pixel_buffer* p_buffer, double delta_time, double total_time);

void lines_bresenham_all_octants(pixel_buffer* p_buffer, double delta_time, double total_time);

void orthographic_cube(pixel_buffer* p_buffer, double delta_time, double total_time);

void isometric_cube(pixel_buffer* p_buffer, double delta_time, double total_time);

void simplified_perspective_cube(pixel_buffer* p_buffer, double delta_time, double total_time);

void simplified_perspective_teapot(pixel_buffer* p_buffer, double delta_time, double total_time);

void rasterization_scanlines_teapot(pixel_buffer* p_buffer, double delta_time, double total_time);

#endif // SCENES_MODULE_H