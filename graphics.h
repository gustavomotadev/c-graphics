#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

// custom pixel buffer (the canvas)
typedef struct pixel_buffer {
    int width;
    int height;
    uint32_t* color_buffer;
} pixel_buffer;

bool setup_pixel_buffer(pixel_buffer* p_buffer, int width, int height);

// Helper function to clear the buffer with a single color
void clear_color_buffer(pixel_buffer* p_buffer, uint32_t color);

// Helper function to draw a single point
void draw_pixel_safe(pixel_buffer* pBuffer, int x, int y, uint32_t color);

#endif // GRAPHICS_H