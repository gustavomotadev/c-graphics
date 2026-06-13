#include "custom_scenes.h"

void checkered_board_animation(pixel_buffer* p_buffer, double delta_time, double total_time) {

    int base_size = 75, variable_size = 25;
    double horizontal_offset = 300.0, vertical_offset = 200.0;
    uint32_t red = 0xFF0000FF, black = 0x000000FF;

    //larger is slower
    double speed_factor = 5.0;

    // turn total time into 0..1 interval
    // x-n*y, where n is the quotient of x/y truncated toward zero to an integer (fmod)
    double value = (total_time - ((int) (total_time/speed_factor))*speed_factor)/speed_factor;
    
    // turn value from 0..1 to -1..1
    value = ((value - 0.5)*2.0);

    // |value| from / to v
    if (value < 0.0) {
        value *= -1.0;
    }

    // flip value from v to ^
    value = 1.0 - value;

    // printf("\ranimation time = %f", value);

    int square_size = base_size + (value)*variable_size;

    for (int x = 0; x < p_buffer->width; x++)
    {
        for (int y = 0; y < p_buffer->height; y++)
        {   
            // checkered board battern
            if (((x+(int)(value*horizontal_offset))/square_size)%2 ^ ((y+(int)(value*vertical_offset))/square_size)%2) 
            {
                draw_pixel(p_buffer, x, y, red);
            }
            else 
            {
                draw_pixel(p_buffer, x, y, black);
            }
        }
        
    }
}

void lines_dda_triangle(pixel_buffer* p_buffer, double delta_time, double total_time) {

    static float x1, y1, x2, y2, x3, y3;
    static double last_time = -1.0;

    if (total_time > last_time + 0.5) {

        x1 = ((float) rand()/RAND_MAX)*(p_buffer->width*3-p_buffer->width);
        x2 = ((float) rand()/RAND_MAX)*(p_buffer->width*3-p_buffer->width);
        x3 = ((float) rand()/RAND_MAX)*(p_buffer->width*3-p_buffer->width);

        y1 = ((float) rand()/RAND_MAX)*(p_buffer->height*3-p_buffer->height);
        y2 = ((float) rand()/RAND_MAX)*(p_buffer->height*3-p_buffer->height);
        y3 = ((float) rand()/RAND_MAX)*(p_buffer->height*3-p_buffer->height);

        last_time = total_time;
    }

    draw_line_dda(p_buffer, x1, y1, x2, y2, 0xFF0000FF);
    draw_line_dda(p_buffer, x2, y2, x3, y3, 0x00FF00FF);
    draw_line_dda(p_buffer, x3, y3, x1, y1, 0x0000FFFF);
}

void lines_bresenham_all_octants(pixel_buffer* p_buffer, double delta_time, double total_time) {

    int x_center = p_buffer->width>>1;
    int y_center = p_buffer->height>>1;

    for (int x = -200; x <= 200; x+=25)
    {
            for (int y = -200; y <= 200; y+=25)
        {
            draw_line_bresenham(p_buffer, x_center, y_center, x_center+x, y_center+y, 0xFF0000FF);
        }
    }

}

void create_test_cube(model* cube) {

    /*
    # Cube with edges of length 2, centered on the origin
    # 8 vertices, 12 triangular faces

    # Vertices
    v -1.0 -1.0  1.0
    v  1.0 -1.0  1.0
    v  1.0  1.0  1.0
    v -1.0  1.0  1.0
    v -1.0 -1.0 -1.0
    v  1.0 -1.0 -1.0
    v  1.0  1.0 -1.0
    v -1.0  1.0 -1.0

    # Faces (counter-clockwise winding order)
    # Front
    f 1 2 3
    f 1 3 4
    # Right
    f 2 6 7
    f 2 7 3
    # Back
    f 6 5 8
    f 6 8 7
    # Left
    f 5 1 4
    f 5 4 8
    # Top
    f 4 3 7
    f 4 7 8
    # Bottom
    f 5 6 2
    f 5 2 1
    */

    cube->num_vertexes = 8;
    cube->num_faces = 12;

    cube->vertexes_3d = (vertex_3d *) malloc(cube->num_vertexes*sizeof(vertex_3d));
    cube->vertexes_3d_transformed = (vertex_3d *) malloc(cube->num_vertexes*sizeof(vertex_3d));
    cube->vertexes_2d = (vertex_2d *) malloc(cube->num_vertexes*sizeof(vertex_2d));
    cube->pixels = (pixel *) malloc(cube->num_vertexes*sizeof(pixel));
    cube->tri_faces = (triangle_face *) malloc(cube->num_faces*sizeof(triangle_face));

    // 0 -1.0 -1.0  1.0
    // 1  1.0 -1.0  1.0
    // 2  1.0  1.0  1.0
    // 3 -1.0  1.0  1.0
    // 4 -1.0 -1.0 -1.0
    // 5  1.0 -1.0 -1.0
    // 6  1.0  1.0 -1.0
    // 7 -1.0  1.0 -1.0

    cube->vertexes_3d[0].x = -1.0f;
    cube->vertexes_3d[0].y = -1.0f;
    cube->vertexes_3d[0].z = 1.0f;
    cube->vertexes_3d[1].x = 1.0f;
    cube->vertexes_3d[1].y = -1.0f;
    cube->vertexes_3d[1].z = 1.0f;
    cube->vertexes_3d[2].x = 1.0f;
    cube->vertexes_3d[2].y = 1.0f;
    cube->vertexes_3d[2].z = 1.0f;
    cube->vertexes_3d[3].x = -1.0f;
    cube->vertexes_3d[3].y = 1.0f;
    cube->vertexes_3d[3].z = 1.0f;
    cube->vertexes_3d[4].x = -1.0f;
    cube->vertexes_3d[4].y = -1.0f;
    cube->vertexes_3d[4].z = -1.0f;
    cube->vertexes_3d[5].x = 1.0f;
    cube->vertexes_3d[5].y = -1.0f;
    cube->vertexes_3d[5].z = -1.0f;
    cube->vertexes_3d[6].x = 1.0f;
    cube->vertexes_3d[6].y = 1.0f;
    cube->vertexes_3d[6].z = -1.0f;
    cube->vertexes_3d[7].x = -1.0f;
    cube->vertexes_3d[7].y = 1.0f;
    cube->vertexes_3d[7].z = -1.0f;

    reset_model_transform(cube);

    // 0 0 1 2
    // 1 0 2 3
    // 2 1 5 6
    // 3 1 6 2
    // 4 5 4 7
    // 5 5 7 6
    // 6 4 0 3
    // 7 4 3 7
    // 8 3 2 6
    // 9 3 6 7
    // 10 4 5 1
    // 11 4 1 0

    cube->tri_faces[0].a = 0;
    cube->tri_faces[0].b = 1;
    cube->tri_faces[0].c = 2;
    cube->tri_faces[1].a = 0;
    cube->tri_faces[1].b = 2;
    cube->tri_faces[1].c = 3;
    cube->tri_faces[2].a = 1;
    cube->tri_faces[2].b = 5;
    cube->tri_faces[2].c = 6;
    cube->tri_faces[3].a = 1;
    cube->tri_faces[3].b = 6;
    cube->tri_faces[3].c = 2;
    cube->tri_faces[4].a = 5;
    cube->tri_faces[4].b = 4;
    cube->tri_faces[4].c = 7;
    cube->tri_faces[5].a = 5;
    cube->tri_faces[5].b = 7;
    cube->tri_faces[5].c = 6;
    cube->tri_faces[6].a = 4;
    cube->tri_faces[6].b = 0;
    cube->tri_faces[6].c = 3;
    cube->tri_faces[7].a = 4;
    cube->tri_faces[7].b = 3;
    cube->tri_faces[7].c = 7;
    cube->tri_faces[8].a = 3;
    cube->tri_faces[8].b = 2;
    cube->tri_faces[8].c = 6;
    cube->tri_faces[9].a = 3;
    cube->tri_faces[9].b = 6;
    cube->tri_faces[9].c = 7;
    cube->tri_faces[10].a = 4;
    cube->tri_faces[10].b = 5;
    cube->tri_faces[10].c = 1;
    cube->tri_faces[11].a = 4;
    cube->tri_faces[11].b = 1;
    cube->tri_faces[11].c = 0;
}

void orthographic_cube(pixel_buffer* p_buffer, double delta_time, double total_time) {

    model cube;
    float deg90 = 3.14159265358979323846f/2.0f;

    create_test_cube(&cube);

    reset_model_transform(&cube);
    rotate_model(&cube, 0.0f, deg90/2.0f, 0.0f);
    scale_model(&cube, 200.0f, 200.0f, 200.0f);
    translate_model(&cube, p_buffer->width/2.0f, p_buffer->height/2.0f, 0.0f);

    draw_model_wireframe(p_buffer, &cube, orthographic_projection, 0x00FF00FF);
}

void isometric_cube(pixel_buffer* p_buffer, double delta_time, double total_time) {

    model cube;
    // float deg90 = 3.14159265358979323846f/2.0f;

    create_test_cube(&cube);

    reset_model_transform(&cube);
    // rotate_model(&cube, deg90/2.0f, deg90/3.0f, deg90/3.0f);
    scale_model(&cube, 100.0f, 100.0f, 100.0f);
    translate_model(&cube, 600.0f, 600.0f, 0.0f);

    draw_model_wireframe(p_buffer, &cube, isometric_projection, 0x00FF00FF);
}