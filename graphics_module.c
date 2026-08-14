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

void vertexes_3d_to_2d(pixel_buffer* p_buffer, model* md, projection_function project) {

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
}

void draw_model(pixel_buffer* p_buffer, model* md, projection_function project, draw_triangle_function draw, uint32_t color) {

    vertexes_3d_to_2d(p_buffer, md, project);

    compute_face_normals(md);

    for (int i = 0; i < md->num_faces; i++)
    {
        // if (!back_face_culling(
        //         md->vertexes_3d_transformed[md->tri_faces[i].a], 
        //         md->vertexes_3d_transformed[md->tri_faces[i].b], 
        //         md->vertexes_3d_transformed[md->tri_faces[i].c], 
        //         md->face_normals[i])) {
        //     continue;
        // }
        
        // debug
        // uint32_t rand_color =  ((uint32_t)(rand() & 0xFFFF) << 16) | ((uint32_t)(rand() & 0xFFFF)) | 0xFF;
        // printf("0x%X, ", rand_color);

        //debug
        uint32_t normal_color = map_normal_to_color(md->face_normals[i]);
        // normal_color &= 0x0000FFFF;
        // printf("%X, ", normal_color);

        draw(p_buffer, 
            md->vertexes_2d[md->tri_faces[i].a].x, 
            md->vertexes_2d[md->tri_faces[i].a].y, 
            md->vertexes_2d[md->tri_faces[i].b].x, 
            md->vertexes_2d[md->tri_faces[i].b].y, 
            md->vertexes_2d[md->tri_faces[i].c].x, 
            md->vertexes_2d[md->tri_faces[i].c].y, normal_color);
    }
    
}

void draw_model_wireframe(pixel_buffer* p_buffer, model* md, projection_function project, uint32_t color) {
    draw_model(p_buffer, md, project, draw_triangle_wireframe, color);
}

void draw_model_scanlines(pixel_buffer* p_buffer, model* md, projection_function project, uint32_t color) {
    draw_model(p_buffer, md, project, rasterize_triangle_scanline, color);
}

void draw_model_edge_functions(pixel_buffer* p_buffer, model* md, projection_function project, uint32_t color) {
    draw_model(p_buffer, md, project, rasterize_triangle_edge_functions, color);
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
    float temp = *x1;
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

    // DEBUG HORIZONTAL BOUNDING BOX
    // float x_min = 0.0f, x_max = 0.0f;
    // if (x1 > x2) {
    //     x_max = (x1 > x3) ? x1 : x3;
    //     x_min = (x2 < x3) ? x2 : x3;
    // } else {
    //     x_max = (x2 > x3) ? x2 : x3;
    //     x_min = (x1 < x3) ? x1 : x3;
    // }
    // int x_min_i = (int) x_min;
    // int x_max_i = (int) ceilf(x_max);
    // printf("X_MIN_I %i, X_MAX_I %i\n", x_min_i, x_max_i);

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
    int y_start = 0, y_end = 0;
    float pre_step = 0.0f;
    int count = 0;
    int x_start = 0, x_end = 0;

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
            // if (ceilf(x_left) < 0 || (int) x_right >= p_buffer->width) {
            //     printf("AFTER  %f %f / %f %f / %f %f\n", x1, y1, x2, y2, x3, y3);
            //     printf("BOTTOM %f %f %i\n", x_left, x_right, y);
            //     printf("SLOPE  %f %f\n", slope_flat_bottom_left, slope_flat_bottom_right);
            // }
            x_start = (int) ceilf(x_left);
            x_end = (int) ceilf(x_right);
            // if (x_start > x_end) printf("SWAPPED!\n"); // debug
            if (x_start <= x_end) {
                // if (x_start < x_min_i || x_end > x_max_i) { //debug
                //     printf("WEIRD LINE\n");
                //     if (x_start < x_min_i) {
                //         printf("LEFT BOUNDARY BROKEN\n");
                //     }
                //     if (x_end > x_max_i) {
                //         printf("RIGHT BOUNDARY BROKEN\n");
                //     }
                //     if (x_start < x_min_i && x_end > x_max_i) {
                //         printf("BOTH BOUNDARIES BROKEN\n");
                //     }
                //     printf("BEFORE SORT %.1f, %.1f ; %.1f, %.1f ; %.1f, %.1f\n", x1, y1, x2, y2, x3, y3);
                //     printf("AFTER SORT  %.1f, %.1f ; %.1f, %.1f ; %.1f, %.1f\n", x1, y1, x2, y2, x3, y3);
                //     printf("BOUNDING_X_LEFT %i, BOUNDING_X_RIGHT %i\n", x_min_i, x_max_i);
                //     printf("FLAT BOTTOM\n");
                //     if (natural_flat_bottom) {
                //         printf("NATURAL FLAT BOTTOM\n");
                //     }
                //     printf("X_DRAW_LEFT %f, X_DRAW_RIGHT %f, Y_DRAW %i\n", x_left, x_right, y);
                //     printf("SLOPE_LEFT %f, SLOPE_RIGHT %f\n", slope_flat_bottom_left, slope_flat_bottom_right);
                //     color = 0xFF0000FF;
                // }

                draw_horizontal_line(p_buffer, x_start, x_end, y, color);
            }
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
            x_start = (int) ceilf(x_left);
            x_end = (int) ceilf(x_right);
            // if (x_start > x_end) printf("SWAPPED!\n"); // debug
            if (x_start <= x_end) {
                // if (x_start < x_min_i || x_end > x_max_i) { //debug
                //     printf("WEIRD LINE\n");
                //     if (x_start < x_min_i) {
                //         printf("LEFT BOUNDARY BROKEN\n");
                //     }
                //     if (x_end > x_max_i) {
                //         printf("RIGHT BOUNDARY BROKEN\n");
                //     }
                //     if (x_start < x_min_i && x_end > x_max_i) {
                //         printf("BOTH BOUNDARIES BROKEN\n");
                //     }
                //     printf("BEFORE SORT %.1f, %.1f ; %.1f, %.1f ; %.1f, %.1f\n", x1, y1, x2, y2, x3, y3);
                //     printf("AFTER SORT  %.1f, %.1f ; %.1f, %.1f ; %.1f, %.1f\n", x1, y1, x2, y2, x3, y3);
                //     printf("BOUNDING_X_LEFT %i, BOUNDING_X_RIGHT %i\n", x_min_i, x_max_i);
                //     printf("FLAT TOP\n");
                //     if (natural_flat_top) {
                //         printf("NATURAL FLAT TOP\n");
                //     }
                //     printf("X_DRAW_LEFT %f, X_DRAW_RIGHT %f, Y_DRAW %i\n", x_left, x_right, y);
                //     printf("SLOPE_LEFT %f, SLOPE_RIGHT %f\n", slope_flat_bottom_left, slope_flat_bottom_right);
                //     color = 0xFF0000FF;
                // }

                draw_horizontal_line(p_buffer, x_start, x_end, y, color);
            }
            x_left = flat_top_left + (slope_flat_top_left * count);
            x_right = flat_top_right + (slope_flat_top_right * count);
            count++;
        }
    }
}

void free_model_data(model* md) {

    free(md->vertexes_3d);
    free(md->vertexes_3d_transformed);
    free(md->vertexes_2d);
    free(md->pixels);
    free(md->tri_faces);
    free(md->face_normals);
}

void compute_face_normals(model* md) {

    for (int i = 0; i < md->num_faces; i++)
    {
        md->face_normals[i] = compute_triangle_normal(
            md->vertexes_3d_transformed[md->tri_faces[i].a],
            md->vertexes_3d_transformed[md->tri_faces[i].b],
            md->vertexes_3d_transformed[md->tri_faces[i].c]);
    }
    
}

uint32_t map_normal_to_color(vector_3d v3d) {

    int r = (int) roundf(((v3d.x*0.5) + 0.5) * 255);
    int g = (int) roundf(((v3d.y*0.5) + 0.5) * 255);
    int b = (int) roundf(((v3d.z*0.5) + 0.5) * 255);

    return (uint32_t) ((r << 24) | (g << 16) | (b << 8) | 0xFF);
}

// assumes the simple perspective projection with camera at (0, 0, 0)
// returns true if face should be drawn, false if culled
bool back_face_culling(vertex_3d p1, vertex_3d p2, vertex_3d p3, vector_3d normal) {

    // center of the face
    vector_3d centroid = compute_triangle_centroid(p1, p2, p3);

    // with the camera assumption, view vector is -Vcentroid [(0,0,0) - Vcentroid]
    vector_3d view_vector = centroid;
    scale_vector_3d(&view_vector, -1.0f, -1.0f, -1.0f);

    // normalizing the view vector is not necessary for culling

    return dot_product_vector_3d(normal, view_vector) > 0 ? true : false;
}

void rasterize_triangle_edge_functions(pixel_buffer* p_buffer, float x1, float y1, float x2, float y2, float x3, float y3, uint32_t color) {

    // compute 2D screen bounding box (clamped):

    float min_x_f, max_x_f, min_y_f, max_y_f;
    int min_x, max_x, min_y, max_y;

    // get max and min at the same time
    minmax3f(x1, x2, x3, &min_x_f, &max_x_f);
    minmax3f(y1, y2, y3, &min_y_f, &max_y_f);

    // discard triangles completely out of screen
    if (max_x_f < 0.0f || 
        min_x_f >= p_buffer->width ||
        max_y_f < 0.0f ||
        min_y_f >= p_buffer->height) {
            return;
        }

    // apply floor or ceil and clamp to screen
    min_x = clampi((int) floorf(min_x_f), 0, p_buffer->width - 1);
    max_x = clampi((int) ceilf(max_x_f), 0, p_buffer->width - 1);
    min_y = clampi((int) floorf(min_y_f), 0, p_buffer->height - 1);
    max_y = clampi((int) ceilf(max_y_f), 0, p_buffer->height - 1);
    //debug
    // printf("1: %.1f %.1f; 2: %.1f %.1f; 3: %.1f %.1f\n", x1, y1, x2, y2, x3, y3);
    // printf("X: %i -> %i ; Y %i -> %i\n", min_x, max_x, min_y, max_y);

    // compute total triangle area * 2 (area of the full parallelogram, magnitude of 2d cross product):

    float triangle_area_x2 = (x2 - x1)*(x3 - x1) - (y2 - y1)*(y3 - y1);
    //debug
    // printf("AX2: %.1f\n", triangle_area_x2);

    // if area <= 0 the triangle is degenerate or flipped (back facing)
    if (lesser_equal_floats(triangle_area_x2, 0.0f, DEFAULT_EPSILON)) {
        return;
    }

    //debug
    // printf("AX2: %.1f\n", triangle_area_x2);

    // edge functions (E23, E31, E12):
    // evaluates which side of the edge a point is on
    // is the 2d cross product: (V1-V0) X (P - V0) ; (signed area of the parallelogram)
    // > 0 right ; = 0 on edge ; < 0 left
    // if a triangle is in CCW order, point is inside if to the right of all edges (>= 0)

    //E23
    float x3_minus_x2 = x3 - x2;
    float y3_minus_y2 = y3 - y2;

    //E31
    float x1_minus_x3 = x1 - x3;
    float y1_minus_y3 = y1 - y3;

    //E12
    float x2_minus_x1 = x2 - x1;
    float y2_minus_y1 = y2 - y1;

    // edge function macros
    #define EDGE23(x, y) ((((x) - x2) * y3_minus_y2) - (((y) - y2) * x3_minus_x2))
    #define EDGE31(x, y) ((((x) - x3) * y1_minus_y3) - (((y) - y3) * x1_minus_x3))
    #define EDGE12(x, y) ((((x) - x1) * y2_minus_y1) - (((y) - y1) * x2_minus_x1))

    // loop over the bounding box:

    for (int x = min_x; x <= max_x; x++)
    {
        for (int y = min_y; y <= max_y; y++)
        {   
            // pixel center
            float x_center = x + 0.5f;
            float y_center = y + 0.5f;

            // if any result is not >= 0, ignore this point
            float edge23 = EDGE23(x_center, y_center);
            //debug
            // printf("[1] E23: %.1f\n", edge23);
            // if (edge23 < 0) {
            //     continue;
            // }
            float edge31 = EDGE31(x_center, y_center);
            // printf("[2] E31: %.1f\n", edge31);
            // if (edge31 < 0) {
            //     continue;
            // }
            float edge12 = EDGE12(x_center, y_center);
            // printf("[3] E12: %.1f\n", edge12);
            // if (edge12 < 0) {
            //     continue;
            // }

            if (((edge23 >= 0) && (edge31 >= 0) && (edge12 >= 0))) {
                draw_pixel(p_buffer, x, y, 0xFF0000FF);
            }
            if (((edge23 <= 0) && (edge31 <= 0) && (edge12 <= 0))) {
                draw_pixel(p_buffer, x, y, 0x00FF00FF);
            }

            // printf("PIXEL DRAW!\n");

            // draw_pixel(p_buffer, x, y, color);
        }
        
    }
    
}