#include "graphics_module.h"

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
void draw_pixel(pixel_buffer* p_buffer, int x, int y, uint32_t color) {
    // Prevent out-of-bounds memory writes (bounds checking)
    if (x >= 0 && x < p_buffer->width && y >= 0 && y < p_buffer->height) {
        p_buffer->color_buffer[(p_buffer->height - 1 - y) * p_buffer->width + x] = color; 
        // Note: (SCREEN_HEIGHT - 1 - y) flips the Y axis so (0,0) is at the bottom-left,
        // which is standard for 3D graphics math.
    }
}

// draw line using Digital differential analyzer algorithm
void draw_line_dda(pixel_buffer* p_buffer, float x1, float y1, float x2, float y2, uint32_t color) {

    float dy = y2-y1;
    float dx = x2-x1;
    int abs_dx = (int) fabsf(dx);
    int abs_dy = (int) fabsf(dy);
    int steps = 0;

    if (abs_dx > abs_dy) {    
        steps = abs_dx;
    } else {
        steps = abs_dy;
    }

    float x_increment = dx / steps;
    float y_increment = dy / steps;

    for (int i = 0; i <= steps; i++)
    {
        int draw_x = (int) fabsf(x1 + (x_increment * i));
        int draw_y = (int) fabsf(y1 + (y_increment * i));
        draw_pixel(p_buffer, draw_x, draw_y, color);
    }
    
}

void draw_line_bresenham(pixel_buffer* p_buffer, int x1, int y1, int x2, int y2, uint32_t color) {

    //REMEMBER: MULTIPLICATION BY 2 TURNS INTO LEFT SHIFT BY 1

    int dx = x2 - x1;
    int dy = y2 - y1;
    int two_dy = dy << 1;
    //initial decision parameter p0
    int decision_parameter = (two_dy) - dx;
    //initial y that will either be mantained or changed by 1 every iteration
    int y = y1;

    int dp_step_change = (dy - dx) << 1;
    int dp_step_mantain = two_dy;

    for (int x = x1; x <= x2; x++) {
        
        draw_pixel(p_buffer, x, y, color);

        //decision parameter is calculated from 2 distances between actual point and upper pixel and lower pixel
        if (decision_parameter > 0) {

            // y changed
            y = y + 1;
            // decision parameter updates
            decision_parameter += dp_step_change;

        } else {

            // y does not change
            // decision parameter updates
            decision_parameter += dp_step_mantain;
        }
    }
    
}