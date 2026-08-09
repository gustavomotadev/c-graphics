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

int static inline xy_to_p_buffer(pixel_buffer* p_buffer, int x, int y) {
    // Note: (SCREEN_HEIGHT - 1 - y) flips the Y axis so (0,0) is at the bottom-left, which is standard for 3D graphics math.
    return (p_buffer->height - 1 - y) * p_buffer->width + x;
}

// Helper function to draw a single point
void draw_pixel(pixel_buffer* p_buffer, int x, int y, uint32_t color) {
    // Prevent out-of-bounds memory writes (bounds checking)
    if (x >= 0 && x < p_buffer->width && y >= 0 && y < p_buffer->height) {
        p_buffer->color_buffer[xy_to_p_buffer(p_buffer, x, y)] = color; 
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

vertex_2d viewport_transform(vertex_2d projected, float half_width, float half_height) {

    vertex_2d transformed;
    
    transformed.x = (projected.x * half_width) + half_width;
    transformed.y = (projected.y * half_height) + half_height;

    return transformed;
}

void draw_triangle_wireframe(pixel_buffer* p_buffer, float x1, float y1, float x2, float y2, float x3, float y3, uint32_t color) {

    int x1i = roundf(x1);
    int x2i = roundf(x2);
    int x3i = roundf(x3);
    int y1i = roundf(y1);
    int y2i = roundf(y2);
    int y3i = roundf(y3);

    draw_line_bresenham(p_buffer, x1i, y1i, x2i, y2i, color);
    draw_line_bresenham(p_buffer, x2i, y2i, x3i, y3i, color);
    draw_line_bresenham(p_buffer, x3i, y3i, x1i, y1i, color);
}

void draw_model(pixel_buffer* p_buffer, model* md, projection_function project, draw_triangle_function draw, uint32_t color) {

    float half_width = (float)p_buffer->width / 2.0f;
    float half_height = (float)p_buffer->height / 2.0f;

    for (int i = 0; i < md->num_vertexes; i++)
    {
        md->vertexes_2d[i] = viewport_transform(project(md->vertexes_3d_transformed[i]), half_width, half_height);
        // printf("[%i] %.2f %.2f\n", i, md->vertexes_2d[i].x, md->vertexes_2d[i].y);
        // TODO: maybe remove pixels array from model completely
        // md->pixels[i] = round_vector_2d(md->vertexes_2d[i]);
        // printf("[%i] %i %i\n", i, md->pixels[i].x, md->pixels[i].y);
    }

    for (int i = 0; i < md->num_faces; i++)
    {
        int a = md->tri_faces[i].a;
        int b = md->tri_faces[i].b;
        int c = md->tri_faces[i].c;
        float x1 = md->vertexes_2d[a].x;
        float y1 = md->vertexes_2d[a].y;
        float x2 = md->vertexes_2d[b].x;
        float y2 = md->vertexes_2d[b].y;
        float x3 = md->vertexes_2d[c].x;
        float y3 = md->vertexes_2d[c].y;
        
        draw(p_buffer, x1, y1, x2, y2, x3, y3, color);
    }
    
}

void draw_model_wireframe(pixel_buffer* p_buffer, model* md, projection_function project, uint32_t color) {
    draw_model(p_buffer, md, project, draw_triangle_wireframe, color);
}

void draw_model_scanlines(pixel_buffer* p_buffer, model* md, projection_function project, uint32_t color) {
    draw_model(p_buffer, md, project, rasterize_triangle_scanline, color);
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

static inline void swap_vertexes_2d(float *x1, float *y1, float *x2, float *y2) {
    int temp = *x1;
    *x1 = *x2;
    *x2 = temp;

    temp = *y1;
    *y1 = *y2;
    *y2 = temp;
}

static inline void swap_int(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

void draw_horizontal_line(pixel_buffer* p_buffer, int x1, int x2, int y, uint32_t color) {

    if (y < 0 || y >= p_buffer->height) {
        return;
    }

    if (x1 == x2) {
        draw_pixel(p_buffer, x1, y, color);
    }

    if (x2 < x1) {
        swap_int(&x1, &x2);
    }

    if (x1 < 0) {
        x1 = 0;
        // printf("HORIZONTAL MIN \n");
    }

    if (x2 >= p_buffer->width) {
        x2 = p_buffer->width - 1;
        // printf("HORIZONTAL MAX\n");
    }

    int start = xy_to_p_buffer(p_buffer, x1, y);
    int end = xy_to_p_buffer(p_buffer, x2, y);

    for (int i = start; i < end; i++) {
        p_buffer->color_buffer[i] = color;
    }
}

// TODO: fix error on negative coordinates or coordinates out of screen
void rasterize_triangle_scanline(pixel_buffer* p_buffer, float x1, float y1, float x2, float y2, float x3, float y3, uint32_t color) {

    float EPSILON = 0.0001;
    
    // printf("BEFORE %.1f %.1f / %.1f %.1f / %.1f %.1f\n", x1, y1, x2, y2, x3, y3);

    // order vertexes 1 <= 2 <= 3 by coordinate y
    if (y1 > y2) swap_vertexes_2d(&x1, &y1, &x2, &y2);
    if (y2 > y3) swap_vertexes_2d(&x2, &y2, &x3, &y3);
    if (y1 > y2) swap_vertexes_2d(&x1, &y1, &x2, &y2);

    // printf("AFTER  %.1f %.1f / %.1f %.1f / %.1f %.1f\n", x1, y1, x2, y2, x3, y3);

    // discard zero height triangles
    if (compare_floats(y1, y3, EPSILON)) return;

    bool natural_flat_bottom = compare_floats(y2, y3, EPSILON);
    bool natural_flat_top = compare_floats(y1, y2, EPSILON);

    // find fourth point that lies in the longest edge (between p1 and p3) with the same y coordinate as middle height point p2 
    // does not need to skip this in case of natural flat top or natural flat bottom, this will never divide by zero because zero height triangles are already discarded
    // also this is important to find what is left and right
    float y4 = y2;
    float x4 = x1 + ((x3-x1)*((y2-y1)/(y3-y1)));

    // // debug
    // draw_pixel(p_buffer, roundf(x4), y4, 0xFFFF00FF);
    // draw_pixel(p_buffer, roundf(x4)-1, y4, 0xFFFF00FF);
    // draw_pixel(p_buffer, roundf(x4)+1, y4, 0xFFFF00FF);
    // draw_pixel(p_buffer, roundf(x4), y4-1, 0xFFFF00FF);
    // draw_pixel(p_buffer, roundf(x4), y4+1, 0xFFFF00FF);
    // draw_pixel(p_buffer, roundf(x4)-2, y4, 0xFFFF00FF);
    // draw_pixel(p_buffer, roundf(x4)+2, y4, 0xFFFF00FF);
    // draw_pixel(p_buffer, roundf(x4), y4-2, 0xFFFF00FF);
    // draw_pixel(p_buffer, roundf(x4), y4+2, 0xFFFF00FF);

    // the line between p2 and p4 is now a horizontal boundary between a flat bottom triangle and a flat top triangle
    // flat bottom: p1, p2, p4 (apex p1, base p2 and p4)
    // flat top: p2, p4, p3 (apex p3, base p2 and p4)

    // calculate inverse slope (dx/dy) to find how much x changes for unit increments of y

    float slope_flat_bottom_p1_p2 = 0.0f;
    float slope_flat_bottom_p1_p4 = 0.0f;
    float slope_flat_top_p2_p3 = 0.0f;
    float slope_flat_top_p4_p3 = 0.0f;
    // skip bottom if natural top
    if (!natural_flat_top) {

        slope_flat_bottom_p1_p2 = (x2-x1)/(y2-y1);
        slope_flat_bottom_p1_p4 = (x4-x1)/(y4-y1);
    }
    // skip top if natural bottom
    if (!natural_flat_bottom) {

        slope_flat_top_p2_p3 = (x3-x2)/(y3-y2);
        slope_flat_top_p4_p3 = (x3-x4)/(y3-y4);
    }

    // check which ones are on the "left" or "right"

    float slope_flat_bottom_left = 0;
    float slope_flat_bottom_right = 0;
    float slope_flat_top_left = 0;
    float slope_flat_top_right = 0;

    float flat_top_left = 0;
    float flat_top_right = 0;

    if (x2 < x4) {

        // p2 on the "left"
        slope_flat_bottom_left = slope_flat_bottom_p1_p2;
        slope_flat_bottom_right = slope_flat_bottom_p1_p4;
        
        slope_flat_top_left = slope_flat_top_p2_p3;
        slope_flat_top_right = slope_flat_top_p4_p3;

        flat_top_left = x2;
        flat_top_right = x4;


    } else {

        // p4 on the "left"
        slope_flat_bottom_left = slope_flat_bottom_p1_p4;
        slope_flat_bottom_right = slope_flat_bottom_p1_p2;

        slope_flat_top_left = slope_flat_top_p4_p3;
        slope_flat_top_right = slope_flat_top_p2_p3;

        flat_top_left = x4;
        flat_top_right = x2;
    }

    // loop each side

    float x_left = 0.0f, x_right = 0.0f;
    int y_start = 0.0f, y_end = 0.0f;
    float pre_step = 0.0f;
    int count = 0;

    // loop flat bottom:
    if (!natural_flat_top) {

        y_start = (int) ceilf(y1);
        y_end = (int) y2;

        pre_step = (float) y_start - y1;

        x_left = x1 + (pre_step * slope_flat_bottom_left);
        x_right = x1 + (pre_step * slope_flat_bottom_right);
        
        count = 1;
        for (int y = y_start; y <= y_end; y++)
        {
            // printf("BOTTOM %.1f %.1f %i\n", x_left, x_right, y);
            // printf("SLOPE  %.1f %.1f\n", slope_flat_bottom_left, slope_flat_bottom_right);
            // if (natural_flat_bottom) printf("BOTTOM %.1f %.1f %i\n", x_left, x_right, y);
            if (ceilf(x_left) < 0 || (int) x_right >= p_buffer->width) {
                printf("AFTER  %f %f / %f %f / %f %f\n", x1, y1, x2, y2, x3, y3);
                printf("BOTTOM %f %f %i\n", x_left, x_right, y);
                printf("SLOPE  %f %f\n", slope_flat_bottom_left, slope_flat_bottom_right);
            }
            draw_horizontal_line(p_buffer, ceilf(x_left), (int) x_right, y, color);
            x_left = x1 + (slope_flat_bottom_left * count);
            x_right = x1 + (slope_flat_bottom_right * count);
            count++;
        }
    }
    
    // loop flat top:
    if (!natural_flat_bottom) {

        y_start = (int) ceilf(y2);
        y_end = (int) y3;

        pre_step = (float) y_start - y2;

        x_left = flat_top_left + (pre_step * slope_flat_top_left);
        x_right = flat_top_right + (pre_step * slope_flat_top_right);
        
        count = 1;
        for (int y = y_start; y <= y_end; y++)
        {
            // printf("TOP    %.1f %.1f %i\n", x_left, x_right, y);
            // printf("SLOPE  %.1f %.1f\n", slope_flat_top_left, slope_flat_top_right);
            // if (natural_flat_top) printf("TOP    %.1f %.1f %i\n", x_left, x_right, y);
            // printf("INTS %i %i\n", (int) ceilf(x_left), (int) x_right);
            // if ((int) ceilf(x_left) < 0 || (int) x_right >= p_buffer->width) {
            //     printf("AFTER  %f %f / %f %f / %f %f\n", x1, y1, x2, y2, x3, y3);
            //     printf("TOP    %f %f %i\n", x_left, x_right, y);
            //     printf("SLOPE  %f %f\n", slope_flat_bottom_left, slope_flat_bottom_right);
            // }
            draw_horizontal_line(p_buffer, ceilf(x_left), (int) x_right, y, color);
            x_left = flat_top_left + (slope_flat_top_left * count);
            x_right = flat_top_right + (slope_flat_top_right * count);
            count++;
        }
    }
}