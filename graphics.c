#include "graphics.h"

bool setup_pixel_buffer(pixel_buffer* p_buffer, int width, int height) {

    p_buffer->width = width;
    p_buffer->height = height;
    // Allocate raw C array
    p_buffer->color_buffer = (uint32_t*) malloc(width * height * sizeof(uint32_t));
    if (!p_buffer->color_buffer) {
        return false;
    }

    return true;
}

// Helper function to clear the buffer with a single color
void clear_color_buffer(pixel_buffer* p_buffer, uint32_t color) {
    for (int i = 0; i < p_buffer->width * p_buffer->height; i++) {
        p_buffer->color_buffer[i] = color;
    }
}

// Helper function to draw a single point
void draw_pixel_safe(pixel_buffer* pBuffer, int x, int y, uint32_t color) {
    // Prevent out-of-bounds memory writes (bounds checking)
    if (x >= 0 && x < pBuffer->width && y >= 0 && y < pBuffer->height) {
        pBuffer->color_buffer[(pBuffer->height - 1 - y) * pBuffer->width + x] = color; 
        // Note: (SCREEN_HEIGHT - 1 - y) flips the Y axis so (0,0) is at the bottom-left,
        // which is standard for 3D graphics math.
    }
}