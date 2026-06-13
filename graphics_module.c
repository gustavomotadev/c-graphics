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

void draw_line_bresenham_shallow(pixel_buffer* p_buffer, int x1, int y1, int x2, int y2, uint32_t color) {

    //REMEMBER: MULTIPLICATION BY 2 TURNS INTO LEFT SHIFT BY 1

    int dx = x2 - x1;
    int dy = y2 - y1;

    // adjustment for the case of negative slope
    int y_step = 1;
    if (dy < 0) {

        y_step = -1;
        dy = -dy;
    }

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

            // y changed (adjusted for negative slope)
            y += y_step;
            // decision parameter updates
            decision_parameter += dp_step_change;

        } else {

            // y does not change
            // decision parameter updates
            decision_parameter += dp_step_mantain;
        }
    }
    
}

void draw_line_bresenham_steep(pixel_buffer* p_buffer, int x1, int y1, int x2, int y2, uint32_t color) {

    //REMEMBER: MULTIPLICATION BY 2 TURNS INTO LEFT SHIFT BY 1

    int dx = x2 - x1;
    int dy = y2 - y1;

    // adjustment for the case of negative slope
    int x_step = 1;
    if (dx < 0) {

        x_step = -1;
        dx = -dx;
    }

    int two_dx = dx << 1;
    //initial decision parameter p0
    int decision_parameter = (two_dx) - dy;
    //initial y that will either be mantained or changed by 1 every iteration
    int x = x1;

    int dp_step_change = (dx - dy) << 1;
    int dp_step_mantain = two_dx;

    for (int y = y1; y <= y2; y++) {
        
        draw_pixel(p_buffer, x, y, color);

        //decision parameter is calculated from 2 distances between actual point and upper pixel and lower pixel
        if (decision_parameter > 0) {

            // y changed (adjusted for negative slope)
            x += x_step;
            // decision parameter updates
            decision_parameter += dp_step_change;

        } else {

            // y does not change
            // decision parameter updates
            decision_parameter += dp_step_mantain;
        }
    }
    
}

void draw_line_bresenham(pixel_buffer* p_buffer, int x1, int y1, int x2, int y2, uint32_t color) {

    int dx = x2 - x1;
    int dy = y2 - y1;
    int abs_dx = dx;
    int abs_dy = dy;
    
    // printf("dx: %i\tdy: %i\n", dx, dy);
    if (dx < 0) {
        abs_dx = -dx;
    }
    
    if (dy < 0) {
        abs_dy = -dy;
    }
    // printf("abs_dx: %i\tabs_dy: %i\n", abs_dx, abs_dy);
    
    if (abs_dy > abs_dx) {

        // steep case
        // printf("steep\n");
        // ensure y1 and y2 are properly ordered
        if (y2 > y1) {

            draw_line_bresenham_steep(p_buffer, x1, y1, x2, y2, color);

        } else {

            draw_line_bresenham_steep(p_buffer, x2, y2, x1, y1, color);
        }
        
    } else {
        
        // shallow case
        // printf("shallow\n");
        // ensure x1 and x2 are properly ordered
        if (x2 > x1) {

            draw_line_bresenham_shallow(p_buffer, x1, y1, x2, y2, color);

        } else {

            draw_line_bresenham_shallow(p_buffer, x2, y2, x1, y1, color);
        }
    }
}

void draw_model_wireframe(pixel_buffer* p_buffer, model* md, projection_function project, uint32_t color) {

    for (int i = 0; i < md->num_vertexes; i++)
    {
        md->vertexes_2d[i] = project(md->vertexes_3d_transformed[i]);
        // printf("[%i] %.2f %.2f\n", i, md->vertexes_2d[i].x, md->vertexes_2d[i].y);
        md->pixels[i] = round_vector_2d(md->vertexes_2d[i]);
        // printf("[%i] %i %i\n", i, md->pixels[i].x, md->pixels[i].y);
    }

    for (int i = 0; i < md->num_faces; i++)
    {
        int a = md->tri_faces[i].a;
        int b = md->tri_faces[i].b;
        int c = md->tri_faces[i].c;
        int x1 = md->pixels[a].x;
        int y1 = md->pixels[a].y;
        int x2 = md->pixels[b].x;
        int y2 = md->pixels[b].y;
        int x3 = md->pixels[c].x;
        int y3 = md->pixels[c].y;
        
        draw_line_bresenham(p_buffer, x1, y1, x2, y2, color);
        draw_line_bresenham(p_buffer, x2, y2, x3, y3, color);
        draw_line_bresenham(p_buffer, x3, y3, x1, y1, color);
    }
    
}

void scale_model(model* md, float factor_x, float factor_y, float factor_z) {

    for (int i = 0; i < md->num_vertexes; i++)
    {
        scale_vector_3d(&md->vertexes_3d_transformed[i], factor_x, factor_y, factor_z);
    }
    
}

void rotate_model(model* md, float angle_x, float angle_y, float angle_z) {

    for (int i = 0; i < md->num_vertexes; i++)
    {
        rotate_vector_3d(&md->vertexes_3d_transformed[i], angle_x, angle_y, angle_z);
    }
}

void translate_model(model* md, float dx, float dy, float dz) {

    for (int i = 0; i < md->num_vertexes; i++)
    {
        translate_vector_3d(&md->vertexes_3d_transformed[i], dx, dy, dz);
    }
}

void reset_model_transform(model* md) {

    memcpy(md->vertexes_3d_transformed, md->vertexes_3d, sizeof(vertex_3d)*md->num_vertexes);
}