#include "custom_scenes.h"

void checkered_board_animation(pixel_buffer* p_buffer, double delta_time, double total_time) {

    int base_size = 75, variable_size = 25;
    double horizontal_offset = 300.0, vertical_offset = 200.0;
    uint32_t red = 0xFF0000FF, black = 0x000000FF;

    //larger is slower
    double speed_factor = 5.0;
    // turn total time into 0..1 interval
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

void create_test_triangle(model* triangle) {

    triangle->num_vertexes = 3;
    triangle->num_faces = 1;

    triangle->vertexes_3d = (vertex_3d *) malloc(triangle->num_vertexes*sizeof(vertex_3d));
    triangle->vertexes_3d_transformed = (vertex_3d *) malloc(triangle->num_vertexes*sizeof(vertex_3d));
    triangle->screen_vertexes = (screen_vertex *) malloc(triangle->num_vertexes*sizeof(screen_vertex));
    // triangle->vertexes_2d = (vertex_2d *) malloc(triangle->num_vertexes*sizeof(vertex_2d));
    // triangle->pixels = (pixel *) malloc(triangle->num_vertexes*sizeof(pixel));
    triangle->tri_faces = (triangle_face *) malloc(triangle->num_faces*sizeof(triangle_face));
    triangle->face_normals = (vector_3d *) malloc(triangle->num_faces*sizeof(vector_3d));

    // static float x1 = 300.0f;
    // static float y1 = 300.0f;
    // static float x2 = 500.0f;
    // static float y2 = 300.0f;
    // static float x3 = 400.0f;
    // static float y3 = 500.0f;

    triangle->vertexes_3d[0].x = -0.5f;
    triangle->vertexes_3d[0].y = -0.5f;
    triangle->vertexes_3d[0].z = 0.0f;
    triangle->vertexes_3d[1].x = 0.5f;
    triangle->vertexes_3d[1].y = -0.5f;
    triangle->vertexes_3d[1].z = 0.0f;
    triangle->vertexes_3d[2].x = 0.0f;
    triangle->vertexes_3d[2].y = 0.5f;
    triangle->vertexes_3d[2].z = 0.0f;

    reset_model_transform(triangle);

    triangle->tri_faces[0].a = 0;
    triangle->tri_faces[0].b = 1;
    triangle->tri_faces[0].c = 2;
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
    cube->vertexes_3d_transformed = (screen_vertex *) malloc(cube->num_vertexes*sizeof(screen_vertex));
    // cube->vertexes_2d = (vertex_2d *) malloc(cube->num_vertexes*sizeof(vertex_2d));
    // cube->pixels = (pixel *) malloc(cube->num_vertexes*sizeof(pixel));
    cube->tri_faces = (triangle_face *) malloc(cube->num_faces*sizeof(triangle_face));
    cube->face_normals = (vector_3d *) malloc(cube->num_faces*sizeof(vector_3d));

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

void simplified_perspective_cube(pixel_buffer* p_buffer, double delta_time, double total_time) {

    model cube;
    double pi = 3.14159265358979323846f;
    // float deg90 = 3.14159265358979323846f/2.0f;

    //larger is slower
    double x_speed_factor = 20.0;
    double y_speed_factor = 10.0;
    double z_speed_factor = 40.0;
    // turn total time into 0..1 interval
    double x_value = (total_time - ((int) (total_time/x_speed_factor))*x_speed_factor)/x_speed_factor;
    double y_value = (total_time - ((int) (total_time/y_speed_factor))*y_speed_factor)/y_speed_factor;
    double z_value = (total_time - ((int) (total_time/z_speed_factor))*z_speed_factor)/z_speed_factor;
    // turn 0..1 into 0..2pi
    x_value = x_value*2.0*pi;
    y_value = y_value*2.0*pi;
    z_value = z_value*2.0*pi;

    create_test_cube(&cube);

    reset_model_transform(&cube);
    rotate_model(&cube, x_value, y_value, z_value);
    // rotate_model(&cube, -x_value, 0.0f, 0.0f);
    // rotate_model(&cube, 0.0f, 0.0f, -z_value);
    
    scale_model(&cube, 0.25f, 0.25f, 0.25f);
    translate_model(&cube, 0.0f, 0.0f, 1.0f);

    draw_model_wireframe(p_buffer, &cube, simplified_perspective_projection, 0x00FF00FF);
}

void create_teapot(model* teapot) {

    teapot->num_vertexes = 137;
    teapot->num_faces = 240;

    teapot->vertexes_3d = (vertex_3d *) malloc(teapot->num_vertexes*sizeof(vertex_3d));
    teapot->vertexes_3d_transformed = (vertex_3d *) malloc(teapot->num_vertexes*sizeof(vertex_3d));
    teapot->screen_vertexes = (screen_vertex *) malloc(teapot->num_vertexes*sizeof(screen_vertex));
    // teapot->vertexes_2d = (vertex_2d *) malloc(teapot->num_vertexes*sizeof(vertex_2d));
    // teapot->pixels = (pixel *) malloc(teapot->num_vertexes*sizeof(pixel));
    teapot->tri_faces = (triangle_face *) malloc(teapot->num_faces*sizeof(triangle_face));
    teapot->face_normals = (vector_3d *) malloc(teapot->num_faces*sizeof(vector_3d));

    teapot->vertexes_3d[0].x = 0.35f;
    teapot->vertexes_3d[0].y = -0.0f;
    teapot->vertexes_3d[0].z = 0.253982f;
    teapot->vertexes_3d[1].x = 0.2485f;
    teapot->vertexes_3d[1].y = -0.2485f;
    teapot->vertexes_3d[1].z = 0.253982f;
    teapot->vertexes_3d[2].x = 0.249055f;
    teapot->vertexes_3d[2].y = -0.249055f;
    teapot->vertexes_3d[2].z = 0.278592f;
    teapot->vertexes_3d[3].x = 0.35078f;
    teapot->vertexes_3d[3].y = -0.0f;
    teapot->vertexes_3d[3].z = 0.278592f;
    teapot->vertexes_3d[4].x = 0.26625f;
    teapot->vertexes_3d[4].y = -0.26625f;
    teapot->vertexes_3d[4].z = 0.253982f;
    teapot->vertexes_3d[5].x = 0.375f;
    teapot->vertexes_3d[5].y = -0.0f;
    teapot->vertexes_3d[5].z = 0.253982f;
    teapot->vertexes_3d[6].x = 0.0f;
    teapot->vertexes_3d[6].y = -0.35f;
    teapot->vertexes_3d[6].z = 0.253982f;
    teapot->vertexes_3d[7].x = 0.0f;
    teapot->vertexes_3d[7].y = -0.35078f;
    teapot->vertexes_3d[7].z = 0.278592f;
    teapot->vertexes_3d[8].x = 0.0f;
    teapot->vertexes_3d[8].y = -0.375f;
    teapot->vertexes_3d[8].z = 0.253982f;
    teapot->vertexes_3d[9].x = -0.256935f;
    teapot->vertexes_3d[9].y = -0.2485f;
    teapot->vertexes_3d[9].z = 0.253982f;
    teapot->vertexes_3d[10].x = -0.25011f;
    teapot->vertexes_3d[10].y = -0.249055f;
    teapot->vertexes_3d[10].z = 0.278592f;
    teapot->vertexes_3d[11].x = -0.26625f;
    teapot->vertexes_3d[11].y = -0.26625f;
    teapot->vertexes_3d[11].z = 0.253982f;
    teapot->vertexes_3d[12].x = -0.35f;
    teapot->vertexes_3d[12].y = -0.0f;
    teapot->vertexes_3d[12].z = 0.253982f;
    teapot->vertexes_3d[13].x = -0.35078f;
    teapot->vertexes_3d[13].y = -0.0f;
    teapot->vertexes_3d[13].z = 0.278592f;
    teapot->vertexes_3d[14].x = -0.375f;
    teapot->vertexes_3d[14].y = -0.0f;
    teapot->vertexes_3d[14].z = 0.253982f;
    teapot->vertexes_3d[15].x = -0.2485f;
    teapot->vertexes_3d[15].y = 0.2485f;
    teapot->vertexes_3d[15].z = 0.253982f;
    teapot->vertexes_3d[16].x = -0.249055f;
    teapot->vertexes_3d[16].y = 0.249055f;
    teapot->vertexes_3d[16].z = 0.278592f;
    teapot->vertexes_3d[17].x = -0.26625f;
    teapot->vertexes_3d[17].y = 0.26625f;
    teapot->vertexes_3d[17].z = 0.253982f;
    teapot->vertexes_3d[18].x = 0.0f;
    teapot->vertexes_3d[18].y = 0.35f;
    teapot->vertexes_3d[18].z = 0.253982f;
    teapot->vertexes_3d[19].x = 0.0f;
    teapot->vertexes_3d[19].y = 0.35078f;
    teapot->vertexes_3d[19].z = 0.278592f;
    teapot->vertexes_3d[20].x = 0.0f;
    teapot->vertexes_3d[20].y = 0.375f;
    teapot->vertexes_3d[20].z = 0.253982f;
    teapot->vertexes_3d[21].x = 0.2485f;
    teapot->vertexes_3d[21].y = 0.2485f;
    teapot->vertexes_3d[21].z = 0.253982f;
    teapot->vertexes_3d[22].x = 0.249055f;
    teapot->vertexes_3d[22].y = 0.249055f;
    teapot->vertexes_3d[22].z = 0.278592f;
    teapot->vertexes_3d[23].x = 0.26625f;
    teapot->vertexes_3d[23].y = 0.26625f;
    teapot->vertexes_3d[23].z = 0.253982f;
    teapot->vertexes_3d[24].x = 0.327265f;
    teapot->vertexes_3d[24].y = -0.327265f;
    teapot->vertexes_3d[24].z = 0.059452f;
    teapot->vertexes_3d[25].x = 0.46094f;
    teapot->vertexes_3d[25].y = -0.0f;
    teapot->vertexes_3d[25].z = 0.059452f;
    teapot->vertexes_3d[26].x = 0.355f;
    teapot->vertexes_3d[26].y = -0.355f;
    teapot->vertexes_3d[26].z = -0.121018f;
    teapot->vertexes_3d[27].x = 0.5f;
    teapot->vertexes_3d[27].y = -0.0f;
    teapot->vertexes_3d[27].z = -0.121018f;
    teapot->vertexes_3d[28].x = 0.0f;
    teapot->vertexes_3d[28].y = -0.46094f;
    teapot->vertexes_3d[28].z = 0.059452f;
    teapot->vertexes_3d[29].x = 0.0f;
    teapot->vertexes_3d[29].y = -0.5f;
    teapot->vertexes_3d[29].z = -0.121018f;
    teapot->vertexes_3d[30].x = -0.327265f;
    teapot->vertexes_3d[30].y = -0.327265f;
    teapot->vertexes_3d[30].z = 0.059452f;
    teapot->vertexes_3d[31].x = -0.355f;
    teapot->vertexes_3d[31].y = -0.355f;
    teapot->vertexes_3d[31].z = -0.121018f;
    teapot->vertexes_3d[32].x = -0.46094f;
    teapot->vertexes_3d[32].y = -0.0f;
    teapot->vertexes_3d[32].z = 0.059452f;
    teapot->vertexes_3d[33].x = -0.5f;
    teapot->vertexes_3d[33].y = -0.0f;
    teapot->vertexes_3d[33].z = -0.121018f;
    teapot->vertexes_3d[34].x = -0.327265f;
    teapot->vertexes_3d[34].y = 0.327265f;
    teapot->vertexes_3d[34].z = 0.059452f;
    teapot->vertexes_3d[35].x = -0.355f;
    teapot->vertexes_3d[35].y = 0.355f;
    teapot->vertexes_3d[35].z = -0.121018f;
    teapot->vertexes_3d[36].x = 0.0f;
    teapot->vertexes_3d[36].y = 0.46094f;
    teapot->vertexes_3d[36].z = 0.059452f;
    teapot->vertexes_3d[37].x = 0.0f;
    teapot->vertexes_3d[37].y = 0.5f;
    teapot->vertexes_3d[37].z = -0.121018f;
    teapot->vertexes_3d[38].x = 0.327265f;
    teapot->vertexes_3d[38].y = 0.327265f;
    teapot->vertexes_3d[38].z = 0.059452f;
    teapot->vertexes_3d[39].x = 0.355f;
    teapot->vertexes_3d[39].y = 0.355f;
    teapot->vertexes_3d[39].z = -0.121018f;
    teapot->vertexes_3d[40].x = 0.310625f;
    teapot->vertexes_3d[40].y = -0.310625f;
    teapot->vertexes_3d[40].z = -0.249923f;
    teapot->vertexes_3d[41].x = 0.4375f;
    teapot->vertexes_3d[41].y = -0.0f;
    teapot->vertexes_3d[41].z = -0.249923f;
    teapot->vertexes_3d[42].x = 0.26625f;
    teapot->vertexes_3d[42].y = -0.26625f;
    teapot->vertexes_3d[42].z = -0.308518f;
    teapot->vertexes_3d[43].x = 0.375f;
    teapot->vertexes_3d[43].y = -0.0f;
    teapot->vertexes_3d[43].z = -0.308518f;
    teapot->vertexes_3d[44].x = 0.0f;
    teapot->vertexes_3d[44].y = -0.4375f;
    teapot->vertexes_3d[44].z = -0.249923f;
    teapot->vertexes_3d[45].x = 0.0f;
    teapot->vertexes_3d[45].y = -0.375f;
    teapot->vertexes_3d[45].z = -0.308518f;
    teapot->vertexes_3d[46].x = -0.310625f;
    teapot->vertexes_3d[46].y = -0.310625f;
    teapot->vertexes_3d[46].z = -0.249923f;
    teapot->vertexes_3d[47].x = -0.26625f;
    teapot->vertexes_3d[47].y = -0.26625f;
    teapot->vertexes_3d[47].z = -0.308518f;
    teapot->vertexes_3d[48].x = -0.4375f;
    teapot->vertexes_3d[48].y = -0.0f;
    teapot->vertexes_3d[48].z = -0.249923f;
    teapot->vertexes_3d[49].x = -0.375f;
    teapot->vertexes_3d[49].y = -0.0f;
    teapot->vertexes_3d[49].z = -0.308518f;
    teapot->vertexes_3d[50].x = -0.310625f;
    teapot->vertexes_3d[50].y = 0.310625f;
    teapot->vertexes_3d[50].z = -0.249923f;
    teapot->vertexes_3d[51].x = -0.26625f;
    teapot->vertexes_3d[51].y = 0.26625f;
    teapot->vertexes_3d[51].z = -0.308518f;
    teapot->vertexes_3d[52].x = 0.0f;
    teapot->vertexes_3d[52].y = 0.4375f;
    teapot->vertexes_3d[52].z = -0.249923f;
    teapot->vertexes_3d[53].x = 0.0f;
    teapot->vertexes_3d[53].y = 0.375f;
    teapot->vertexes_3d[53].z = -0.308518f;
    teapot->vertexes_3d[54].x = 0.310625f;
    teapot->vertexes_3d[54].y = 0.310625f;
    teapot->vertexes_3d[54].z = -0.249923f;
    teapot->vertexes_3d[55].x = 0.26625f;
    teapot->vertexes_3d[55].y = 0.26625f;
    teapot->vertexes_3d[55].z = -0.308518f;
    teapot->vertexes_3d[56].x = 0.227975f;
    teapot->vertexes_3d[56].y = -0.227975f;
    teapot->vertexes_3d[56].z = -0.334298f;
    teapot->vertexes_3d[57].x = 0.321095f;
    teapot->vertexes_3d[57].y = -0.0f;
    teapot->vertexes_3d[57].z = -0.334298f;
    teapot->vertexes_3d[58].x = 0.0f;
    teapot->vertexes_3d[58].y = 0.0f;
    teapot->vertexes_3d[58].z = -0.346018f;
    teapot->vertexes_3d[59].x = 0.0f;
    teapot->vertexes_3d[59].y = -0.321095f;
    teapot->vertexes_3d[59].z = -0.334298f;
    teapot->vertexes_3d[60].x = -0.227975f;
    teapot->vertexes_3d[60].y = -0.227975f;
    teapot->vertexes_3d[60].z = -0.334298f;
    teapot->vertexes_3d[61].x = -0.321095f;
    teapot->vertexes_3d[61].y = -0.0f;
    teapot->vertexes_3d[61].z = -0.334298f;
    teapot->vertexes_3d[62].x = -0.227975f;
    teapot->vertexes_3d[62].y = 0.227975f;
    teapot->vertexes_3d[62].z = -0.334298f;
    teapot->vertexes_3d[63].x = 0.0f;
    teapot->vertexes_3d[63].y = 0.321095f;
    teapot->vertexes_3d[63].z = -0.334298f;
    teapot->vertexes_3d[64].x = 0.227975f;
    teapot->vertexes_3d[64].y = 0.227975f;
    teapot->vertexes_3d[64].z = -0.334298f;
    teapot->vertexes_3d[65].x = -0.4f;
    teapot->vertexes_3d[65].y = -0.0f;
    teapot->vertexes_3d[65].z = 0.160232f;
    teapot->vertexes_3d[66].x = -0.3875f;
    teapot->vertexes_3d[66].y = -0.05625f;
    teapot->vertexes_3d[66].z = 0.188357f;
    teapot->vertexes_3d[67].x = -0.62969f;
    teapot->vertexes_3d[67].y = -0.05625f;
    teapot->vertexes_3d[67].z = 0.177812f;
    teapot->vertexes_3d[68].x = -0.603125f;
    teapot->vertexes_3d[68].y = -0.0f;
    teapot->vertexes_3d[68].z = 0.153202f;
    teapot->vertexes_3d[69].x = -0.7125f;
    teapot->vertexes_3d[69].y = -0.05625f;
    teapot->vertexes_3d[69].z = 0.103982f;
    teapot->vertexes_3d[70].x = -0.675f;
    teapot->vertexes_3d[70].y = -0.0f;
    teapot->vertexes_3d[70].z = 0.103982f;
    teapot->vertexes_3d[71].x = -0.375f;
    teapot->vertexes_3d[71].y = -0.0f;
    teapot->vertexes_3d[71].z = 0.216482f;
    teapot->vertexes_3d[72].x = -0.65625f;
    teapot->vertexes_3d[72].y = -0.0f;
    teapot->vertexes_3d[72].z = 0.202422f;
    teapot->vertexes_3d[73].x = -0.75f;
    teapot->vertexes_3d[73].y = -0.0f;
    teapot->vertexes_3d[73].z = 0.103982f;
    teapot->vertexes_3d[74].x = -0.3875f;
    teapot->vertexes_3d[74].y = 0.05625f;
    teapot->vertexes_3d[74].z = 0.188357f;
    teapot->vertexes_3d[75].x = -0.62969f;
    teapot->vertexes_3d[75].y = 0.05625f;
    teapot->vertexes_3d[75].z = 0.177812f;
    teapot->vertexes_3d[76].x = -0.7125f;
    teapot->vertexes_3d[76].y = 0.05625f;
    teapot->vertexes_3d[76].z = 0.103982f;
    teapot->vertexes_3d[77].x = -0.658595f;
    teapot->vertexes_3d[77].y = -0.05625f;
    teapot->vertexes_3d[77].z = -0.032543f;
    teapot->vertexes_3d[78].x = -0.634375f;
    teapot->vertexes_3d[78].y = -0.0f;
    teapot->vertexes_3d[78].z = -0.008518f;
    teapot->vertexes_3d[79].x = -0.4875f;
    teapot->vertexes_3d[79].y = -0.05625f;
    teapot->vertexes_3d[79].z = -0.158518f;
    teapot->vertexes_3d[80].x = -0.682815f;
    teapot->vertexes_3d[80].y = -0.0f;
    teapot->vertexes_3d[80].z = -0.056563f;
    teapot->vertexes_3d[81].x = -0.475f;
    teapot->vertexes_3d[81].y = -0.0f;
    teapot->vertexes_3d[81].z = -0.196018f;
    teapot->vertexes_3d[82].x = -0.658595f;
    teapot->vertexes_3d[82].y = 0.05625f;
    teapot->vertexes_3d[82].z = -0.032543f;
    teapot->vertexes_3d[83].x = -0.4875f;
    teapot->vertexes_3d[83].y = 0.05625f;
    teapot->vertexes_3d[83].z = -0.158518f;
    teapot->vertexes_3d[84].x = 0.425f;
    teapot->vertexes_3d[84].y = -0.0f;
    teapot->vertexes_3d[84].z = 0.010232f;
    teapot->vertexes_3d[85].x = 0.425f;
    teapot->vertexes_3d[85].y = -0.12375f;
    teapot->vertexes_3d[85].z = -0.092893f;
    teapot->vertexes_3d[86].x = 0.634375f;
    teapot->vertexes_3d[86].y = -0.08531f;
    teapot->vertexes_3d[86].z = 0.059452f;
    teapot->vertexes_3d[87].x = 0.596875f;
    teapot->vertexes_3d[87].y = -0.0f;
    teapot->vertexes_3d[87].z = 0.103982f;
    teapot->vertexes_3d[88].x = 0.75f;
    teapot->vertexes_3d[88].y = -0.046875f;
    teapot->vertexes_3d[88].z = 0.253982f;
    teapot->vertexes_3d[89].x = 0.675f;
    teapot->vertexes_3d[89].y = -0.0f;
    teapot->vertexes_3d[89].z = 0.253982f;
    teapot->vertexes_3d[90].x = 0.425f;
    teapot->vertexes_3d[90].y = -0.0f;
    teapot->vertexes_3d[90].z = -0.196018f;
    teapot->vertexes_3d[91].x = 0.671875f;
    teapot->vertexes_3d[91].y = -0.0f;
    teapot->vertexes_3d[91].z = 0.014917f;
    teapot->vertexes_3d[92].x = 0.825f;
    teapot->vertexes_3d[92].y = -0.0f;
    teapot->vertexes_3d[92].z = 0.253982f;
    teapot->vertexes_3d[93].x = 0.425f;
    teapot->vertexes_3d[93].y = 0.12375f;
    teapot->vertexes_3d[93].z = -0.092893f;
    teapot->vertexes_3d[94].x = 0.634375f;
    teapot->vertexes_3d[94].y = 0.08531f;
    teapot->vertexes_3d[94].z = 0.059452f;
    teapot->vertexes_3d[95].x = 0.75f;
    teapot->vertexes_3d[95].y = 0.046875f;
    teapot->vertexes_3d[95].z = 0.253982f;
    teapot->vertexes_3d[96].x = 0.78164f;
    teapot->vertexes_3d[96].y = -0.0375f;
    teapot->vertexes_3d[96].z = 0.270682f;
    teapot->vertexes_3d[97].x = 0.70625f;
    teapot->vertexes_3d[97].y = -0.0f;
    teapot->vertexes_3d[97].z = 0.268047f;
    teapot->vertexes_3d[98].x = 0.75f;
    teapot->vertexes_3d[98].y = -0.028125f;
    teapot->vertexes_3d[98].z = 0.253982f;
    teapot->vertexes_3d[99].x = 0.7f;
    teapot->vertexes_3d[99].y = -0.0f;
    teapot->vertexes_3d[99].z = 0.253982f;
    teapot->vertexes_3d[100].x = 0.85703f;
    teapot->vertexes_3d[100].y = -0.0f;
    teapot->vertexes_3d[100].z = 0.273317f;
    teapot->vertexes_3d[101].x = 0.8f;
    teapot->vertexes_3d[101].y = -0.0f;
    teapot->vertexes_3d[101].z = 0.253982f;
    teapot->vertexes_3d[102].x = 0.78164f;
    teapot->vertexes_3d[102].y = 0.0375f;
    teapot->vertexes_3d[102].z = 0.270682f;
    teapot->vertexes_3d[103].x = 0.75f;
    teapot->vertexes_3d[103].y = 0.028125f;
    teapot->vertexes_3d[103].z = 0.253982f;
    teapot->vertexes_3d[104].x = 0.05776f;
    teapot->vertexes_3d[104].y = -0.05776f;
    teapot->vertexes_3d[104].z = 0.399297f;
    teapot->vertexes_3d[105].x = 0.08125f;
    teapot->vertexes_3d[105].y = -0.0f;
    teapot->vertexes_3d[105].z = 0.399297f;
    teapot->vertexes_3d[106].x = 0.0f;
    teapot->vertexes_3d[106].y = -0.0f;
    teapot->vertexes_3d[106].z = 0.441482f;
    teapot->vertexes_3d[107].x = 0.0355f;
    teapot->vertexes_3d[107].y = -0.0355f;
    teapot->vertexes_3d[107].z = 0.328982f;
    teapot->vertexes_3d[108].x = 0.05f;
    teapot->vertexes_3d[108].y = -0.0f;
    teapot->vertexes_3d[108].z = 0.328982f;
    teapot->vertexes_3d[109].x = 0.0f;
    teapot->vertexes_3d[109].y = -0.08125f;
    teapot->vertexes_3d[109].z = 0.399297f;
    teapot->vertexes_3d[110].x = 0.0f;
    teapot->vertexes_3d[110].y = -0.05f;
    teapot->vertexes_3d[110].z = 0.328982f;
    teapot->vertexes_3d[111].x = -0.05776f;
    teapot->vertexes_3d[111].y = -0.05776f;
    teapot->vertexes_3d[111].z = 0.399297f;
    teapot->vertexes_3d[112].x = -0.0355f;
    teapot->vertexes_3d[112].y = -0.0355f;
    teapot->vertexes_3d[112].z = 0.328982f;
    teapot->vertexes_3d[113].x = -0.08125f;
    teapot->vertexes_3d[113].y = -0.0f;
    teapot->vertexes_3d[113].z = 0.399297f;
    teapot->vertexes_3d[114].x = -0.05f;
    teapot->vertexes_3d[114].y = -0.0f;
    teapot->vertexes_3d[114].z = 0.328982f;
    teapot->vertexes_3d[115].x = -0.05776f;
    teapot->vertexes_3d[115].y = 0.05776f;
    teapot->vertexes_3d[115].z = 0.399297f;
    teapot->vertexes_3d[116].x = -0.0355f;
    teapot->vertexes_3d[116].y = 0.0355f;
    teapot->vertexes_3d[116].z = 0.328982f;
    teapot->vertexes_3d[117].x = 0.0f;
    teapot->vertexes_3d[117].y = 0.08125f;
    teapot->vertexes_3d[117].z = 0.399297f;
    teapot->vertexes_3d[118].x = 0.0f;
    teapot->vertexes_3d[118].y = 0.05f;
    teapot->vertexes_3d[118].z = 0.328982f;
    teapot->vertexes_3d[119].x = 0.05776f;
    teapot->vertexes_3d[119].y = 0.05776f;
    teapot->vertexes_3d[119].z = 0.399297f;
    teapot->vertexes_3d[120].x = 0.0355f;
    teapot->vertexes_3d[120].y = 0.0355f;
    teapot->vertexes_3d[120].z = 0.328982f;
    teapot->vertexes_3d[121].x = 0.14644f;
    teapot->vertexes_3d[121].y = -0.14644f;
    teapot->vertexes_3d[121].z = 0.291482f;
    teapot->vertexes_3d[122].x = 0.20625f;
    teapot->vertexes_3d[122].y = -0.0f;
    teapot->vertexes_3d[122].z = 0.291482f;
    teapot->vertexes_3d[123].x = 0.23075f;
    teapot->vertexes_3d[123].y = -0.23075f;
    teapot->vertexes_3d[123].z = 0.253982f;
    teapot->vertexes_3d[124].x = 0.325f;
    teapot->vertexes_3d[124].y = -0.0f;
    teapot->vertexes_3d[124].z = 0.253982f;
    teapot->vertexes_3d[125].x = 0.0f;
    teapot->vertexes_3d[125].y = -0.20625f;
    teapot->vertexes_3d[125].z = 0.291482f;
    teapot->vertexes_3d[126].x = 0.0f;
    teapot->vertexes_3d[126].y = -0.325f;
    teapot->vertexes_3d[126].z = 0.253982f;
    teapot->vertexes_3d[127].x = -0.14644f;
    teapot->vertexes_3d[127].y = -0.14644f;
    teapot->vertexes_3d[127].z = 0.291482f;
    teapot->vertexes_3d[128].x = -0.23075f;
    teapot->vertexes_3d[128].y = -0.23075f;
    teapot->vertexes_3d[128].z = 0.253982f;
    teapot->vertexes_3d[129].x = -0.20625f;
    teapot->vertexes_3d[129].y = -0.0f;
    teapot->vertexes_3d[129].z = 0.291482f;
    teapot->vertexes_3d[130].x = -0.325f;
    teapot->vertexes_3d[130].y = -0.0f;
    teapot->vertexes_3d[130].z = 0.253982f;
    teapot->vertexes_3d[131].x = -0.14644f;
    teapot->vertexes_3d[131].y = 0.14644f;
    teapot->vertexes_3d[131].z = 0.291482f;
    teapot->vertexes_3d[132].x = -0.23075f;
    teapot->vertexes_3d[132].y = 0.23075f;
    teapot->vertexes_3d[132].z = 0.253982f;
    teapot->vertexes_3d[133].x = 0.0f;
    teapot->vertexes_3d[133].y = 0.20625f;
    teapot->vertexes_3d[133].z = 0.291482f;
    teapot->vertexes_3d[134].x = 0.0f;
    teapot->vertexes_3d[134].y = 0.325f;
    teapot->vertexes_3d[134].z = 0.253982f;
    teapot->vertexes_3d[135].x = 0.14644f;
    teapot->vertexes_3d[135].y = 0.14644f;
    teapot->vertexes_3d[135].z = 0.291482f;
    teapot->vertexes_3d[136].x = 0.23075f;
    teapot->vertexes_3d[136].y = 0.23075f;
    teapot->vertexes_3d[136].z = 0.253982f;

    reset_model_transform(teapot);

    teapot->tri_faces[0].a = 1;
    teapot->tri_faces[0].b = 3;
    teapot->tri_faces[0].c = 0;
    teapot->tri_faces[1].a = 2;
    teapot->tri_faces[1].b = 5;
    teapot->tri_faces[1].c = 3;
    teapot->tri_faces[2].a = 1;
    teapot->tri_faces[2].b = 7;
    teapot->tri_faces[2].c = 2;
    teapot->tri_faces[3].a = 2;
    teapot->tri_faces[3].b = 8;
    teapot->tri_faces[3].c = 4;
    teapot->tri_faces[4].a = 6;
    teapot->tri_faces[4].b = 10;
    teapot->tri_faces[4].c = 7;
    teapot->tri_faces[5].a = 7;
    teapot->tri_faces[5].b = 11;
    teapot->tri_faces[5].c = 8;
    teapot->tri_faces[6].a = 9;
    teapot->tri_faces[6].b = 13;
    teapot->tri_faces[6].c = 10;
    teapot->tri_faces[7].a = 10;
    teapot->tri_faces[7].b = 14;
    teapot->tri_faces[7].c = 11;
    teapot->tri_faces[8].a = 15;
    teapot->tri_faces[8].b = 13;
    teapot->tri_faces[8].c = 12;
    teapot->tri_faces[9].a = 16;
    teapot->tri_faces[9].b = 14;
    teapot->tri_faces[9].c = 13;
    teapot->tri_faces[10].a = 15;
    teapot->tri_faces[10].b = 19;
    teapot->tri_faces[10].c = 16;
    teapot->tri_faces[11].a = 16;
    teapot->tri_faces[11].b = 20;
    teapot->tri_faces[11].c = 17;
    teapot->tri_faces[12].a = 21;
    teapot->tri_faces[12].b = 19;
    teapot->tri_faces[12].c = 18;
    teapot->tri_faces[13].a = 22;
    teapot->tri_faces[13].b = 20;
    teapot->tri_faces[13].c = 19;
    teapot->tri_faces[14].a = 21;
    teapot->tri_faces[14].b = 3;
    teapot->tri_faces[14].c = 22;
    teapot->tri_faces[15].a = 22;
    teapot->tri_faces[15].b = 5;
    teapot->tri_faces[15].c = 23;
    teapot->tri_faces[16].a = 4;
    teapot->tri_faces[16].b = 25;
    teapot->tri_faces[16].c = 5;
    teapot->tri_faces[17].a = 24;
    teapot->tri_faces[17].b = 27;
    teapot->tri_faces[17].c = 25;
    teapot->tri_faces[18].a = 4;
    teapot->tri_faces[18].b = 28;
    teapot->tri_faces[18].c = 24;
    teapot->tri_faces[19].a = 24;
    teapot->tri_faces[19].b = 29;
    teapot->tri_faces[19].c = 26;
    teapot->tri_faces[20].a = 11;
    teapot->tri_faces[20].b = 28;
    teapot->tri_faces[20].c = 8;
    teapot->tri_faces[21].a = 30;
    teapot->tri_faces[21].b = 29;
    teapot->tri_faces[21].c = 28;
    teapot->tri_faces[22].a = 11;
    teapot->tri_faces[22].b = 32;
    teapot->tri_faces[22].c = 30;
    teapot->tri_faces[23].a = 30;
    teapot->tri_faces[23].b = 33;
    teapot->tri_faces[23].c = 31;
    teapot->tri_faces[24].a = 17;
    teapot->tri_faces[24].b = 32;
    teapot->tri_faces[24].c = 14;
    teapot->tri_faces[25].a = 34;
    teapot->tri_faces[25].b = 33;
    teapot->tri_faces[25].c = 32;
    teapot->tri_faces[26].a = 17;
    teapot->tri_faces[26].b = 36;
    teapot->tri_faces[26].c = 34;
    teapot->tri_faces[27].a = 34;
    teapot->tri_faces[27].b = 37;
    teapot->tri_faces[27].c = 35;
    teapot->tri_faces[28].a = 23;
    teapot->tri_faces[28].b = 36;
    teapot->tri_faces[28].c = 20;
    teapot->tri_faces[29].a = 38;
    teapot->tri_faces[29].b = 37;
    teapot->tri_faces[29].c = 36;
    teapot->tri_faces[30].a = 23;
    teapot->tri_faces[30].b = 25;
    teapot->tri_faces[30].c = 38;
    teapot->tri_faces[31].a = 38;
    teapot->tri_faces[31].b = 27;
    teapot->tri_faces[31].c = 39;
    teapot->tri_faces[32].a = 27;
    teapot->tri_faces[32].b = 40;
    teapot->tri_faces[32].c = 41;
    teapot->tri_faces[33].a = 41;
    teapot->tri_faces[33].b = 42;
    teapot->tri_faces[33].c = 43;
    teapot->tri_faces[34].a = 29;
    teapot->tri_faces[34].b = 40;
    teapot->tri_faces[34].c = 26;
    teapot->tri_faces[35].a = 44;
    teapot->tri_faces[35].b = 42;
    teapot->tri_faces[35].c = 40;
    teapot->tri_faces[36].a = 29;
    teapot->tri_faces[36].b = 46;
    teapot->tri_faces[36].c = 44;
    teapot->tri_faces[37].a = 44;
    teapot->tri_faces[37].b = 47;
    teapot->tri_faces[37].c = 45;
    teapot->tri_faces[38].a = 33;
    teapot->tri_faces[38].b = 46;
    teapot->tri_faces[38].c = 31;
    teapot->tri_faces[39].a = 48;
    teapot->tri_faces[39].b = 47;
    teapot->tri_faces[39].c = 46;
    teapot->tri_faces[40].a = 33;
    teapot->tri_faces[40].b = 50;
    teapot->tri_faces[40].c = 48;
    teapot->tri_faces[41].a = 48;
    teapot->tri_faces[41].b = 51;
    teapot->tri_faces[41].c = 49;
    teapot->tri_faces[42].a = 37;
    teapot->tri_faces[42].b = 50;
    teapot->tri_faces[42].c = 35;
    teapot->tri_faces[43].a = 52;
    teapot->tri_faces[43].b = 51;
    teapot->tri_faces[43].c = 50;
    teapot->tri_faces[44].a = 37;
    teapot->tri_faces[44].b = 54;
    teapot->tri_faces[44].c = 52;
    teapot->tri_faces[45].a = 52;
    teapot->tri_faces[45].b = 55;
    teapot->tri_faces[45].c = 53;
    teapot->tri_faces[46].a = 27;
    teapot->tri_faces[46].b = 54;
    teapot->tri_faces[46].c = 39;
    teapot->tri_faces[47].a = 41;
    teapot->tri_faces[47].b = 55;
    teapot->tri_faces[47].c = 54;
    teapot->tri_faces[48].a = 43;
    teapot->tri_faces[48].b = 56;
    teapot->tri_faces[48].c = 57;
    teapot->tri_faces[49].a = 57;
    teapot->tri_faces[49].b = 56;
    teapot->tri_faces[49].c = 58;
    teapot->tri_faces[50].a = 45;
    teapot->tri_faces[50].b = 56;
    teapot->tri_faces[50].c = 42;
    teapot->tri_faces[51].a = 56;
    teapot->tri_faces[51].b = 59;
    teapot->tri_faces[51].c = 58;
    teapot->tri_faces[52].a = 45;
    teapot->tri_faces[52].b = 60;
    teapot->tri_faces[52].c = 59;
    teapot->tri_faces[53].a = 59;
    teapot->tri_faces[53].b = 60;
    teapot->tri_faces[53].c = 58;
    teapot->tri_faces[54].a = 49;
    teapot->tri_faces[54].b = 60;
    teapot->tri_faces[54].c = 47;
    teapot->tri_faces[55].a = 60;
    teapot->tri_faces[55].b = 61;
    teapot->tri_faces[55].c = 58;
    teapot->tri_faces[56].a = 49;
    teapot->tri_faces[56].b = 62;
    teapot->tri_faces[56].c = 61;
    teapot->tri_faces[57].a = 61;
    teapot->tri_faces[57].b = 62;
    teapot->tri_faces[57].c = 58;
    teapot->tri_faces[58].a = 53;
    teapot->tri_faces[58].b = 62;
    teapot->tri_faces[58].c = 51;
    teapot->tri_faces[59].a = 62;
    teapot->tri_faces[59].b = 63;
    teapot->tri_faces[59].c = 58;
    teapot->tri_faces[60].a = 53;
    teapot->tri_faces[60].b = 64;
    teapot->tri_faces[60].c = 63;
    teapot->tri_faces[61].a = 63;
    teapot->tri_faces[61].b = 64;
    teapot->tri_faces[61].c = 58;
    teapot->tri_faces[62].a = 43;
    teapot->tri_faces[62].b = 64;
    teapot->tri_faces[62].c = 55;
    teapot->tri_faces[63].a = 64;
    teapot->tri_faces[63].b = 57;
    teapot->tri_faces[63].c = 58;
    teapot->tri_faces[64].a = 66;
    teapot->tri_faces[64].b = 68;
    teapot->tri_faces[64].c = 65;
    teapot->tri_faces[65].a = 67;
    teapot->tri_faces[65].b = 70;
    teapot->tri_faces[65].c = 68;
    teapot->tri_faces[66].a = 71;
    teapot->tri_faces[66].b = 67;
    teapot->tri_faces[66].c = 66;
    teapot->tri_faces[67].a = 72;
    teapot->tri_faces[67].b = 69;
    teapot->tri_faces[67].c = 67;
    teapot->tri_faces[68].a = 71;
    teapot->tri_faces[68].b = 75;
    teapot->tri_faces[68].c = 72;
    teapot->tri_faces[69].a = 72;
    teapot->tri_faces[69].b = 76;
    teapot->tri_faces[69].c = 73;
    teapot->tri_faces[70].a = 74;
    teapot->tri_faces[70].b = 68;
    teapot->tri_faces[70].c = 75;
    teapot->tri_faces[71].a = 75;
    teapot->tri_faces[71].b = 70;
    teapot->tri_faces[71].c = 76;
    teapot->tri_faces[72].a = 69;
    teapot->tri_faces[72].b = 78;
    teapot->tri_faces[72].c = 70;
    teapot->tri_faces[73].a = 77;
    teapot->tri_faces[73].b = 33;
    teapot->tri_faces[73].c = 78;
    teapot->tri_faces[74].a = 69;
    teapot->tri_faces[74].b = 80;
    teapot->tri_faces[74].c = 77;
    teapot->tri_faces[75].a = 80;
    teapot->tri_faces[75].b = 79;
    teapot->tri_faces[75].c = 77;
    teapot->tri_faces[76].a = 76;
    teapot->tri_faces[76].b = 80;
    teapot->tri_faces[76].c = 73;
    teapot->tri_faces[77].a = 80;
    teapot->tri_faces[77].b = 83;
    teapot->tri_faces[77].c = 81;
    teapot->tri_faces[78].a = 76;
    teapot->tri_faces[78].b = 78;
    teapot->tri_faces[78].c = 82;
    teapot->tri_faces[79].a = 82;
    teapot->tri_faces[79].b = 33;
    teapot->tri_faces[79].c = 83;
    teapot->tri_faces[80].a = 84;
    teapot->tri_faces[80].b = 86;
    teapot->tri_faces[80].c = 87;
    teapot->tri_faces[81].a = 86;
    teapot->tri_faces[81].b = 89;
    teapot->tri_faces[81].c = 87;
    teapot->tri_faces[82].a = 85;
    teapot->tri_faces[82].b = 91;
    teapot->tri_faces[82].c = 86;
    teapot->tri_faces[83].a = 91;
    teapot->tri_faces[83].b = 88;
    teapot->tri_faces[83].c = 86;
    teapot->tri_faces[84].a = 93;
    teapot->tri_faces[84].b = 91;
    teapot->tri_faces[84].c = 90;
    teapot->tri_faces[85].a = 91;
    teapot->tri_faces[85].b = 95;
    teapot->tri_faces[85].c = 92;
    teapot->tri_faces[86].a = 84;
    teapot->tri_faces[86].b = 94;
    teapot->tri_faces[86].c = 93;
    teapot->tri_faces[87].a = 94;
    teapot->tri_faces[87].b = 89;
    teapot->tri_faces[87].c = 95;
    teapot->tri_faces[88].a = 88;
    teapot->tri_faces[88].b = 97;
    teapot->tri_faces[88].c = 89;
    teapot->tri_faces[89].a = 97;
    teapot->tri_faces[89].b = 98;
    teapot->tri_faces[89].c = 99;
    teapot->tri_faces[90].a = 92;
    teapot->tri_faces[90].b = 96;
    teapot->tri_faces[90].c = 88;
    teapot->tri_faces[91].a = 96;
    teapot->tri_faces[91].b = 101;
    teapot->tri_faces[91].c = 98;
    teapot->tri_faces[92].a = 92;
    teapot->tri_faces[92].b = 102;
    teapot->tri_faces[92].c = 100;
    teapot->tri_faces[93].a = 102;
    teapot->tri_faces[93].b = 101;
    teapot->tri_faces[93].c = 100;
    teapot->tri_faces[94].a = 95;
    teapot->tri_faces[94].b = 97;
    teapot->tri_faces[94].c = 102;
    teapot->tri_faces[95].a = 97;
    teapot->tri_faces[95].b = 103;
    teapot->tri_faces[95].c = 102;
    teapot->tri_faces[96].a = 104;
    teapot->tri_faces[96].b = 105;
    teapot->tri_faces[96].c = 106;
    teapot->tri_faces[97].a = 105;
    teapot->tri_faces[97].b = 107;
    teapot->tri_faces[97].c = 108;
    teapot->tri_faces[98].a = 109;
    teapot->tri_faces[98].b = 104;
    teapot->tri_faces[98].c = 106;
    teapot->tri_faces[99].a = 109;
    teapot->tri_faces[99].b = 107;
    teapot->tri_faces[99].c = 104;
    teapot->tri_faces[100].a = 111;
    teapot->tri_faces[100].b = 109;
    teapot->tri_faces[100].c = 106;
    teapot->tri_faces[101].a = 109;
    teapot->tri_faces[101].b = 112;
    teapot->tri_faces[101].c = 110;
    teapot->tri_faces[102].a = 113;
    teapot->tri_faces[102].b = 111;
    teapot->tri_faces[102].c = 106;
    teapot->tri_faces[103].a = 113;
    teapot->tri_faces[103].b = 112;
    teapot->tri_faces[103].c = 111;
    teapot->tri_faces[104].a = 115;
    teapot->tri_faces[104].b = 113;
    teapot->tri_faces[104].c = 106;
    teapot->tri_faces[105].a = 113;
    teapot->tri_faces[105].b = 116;
    teapot->tri_faces[105].c = 114;
    teapot->tri_faces[106].a = 117;
    teapot->tri_faces[106].b = 115;
    teapot->tri_faces[106].c = 106;
    teapot->tri_faces[107].a = 117;
    teapot->tri_faces[107].b = 116;
    teapot->tri_faces[107].c = 115;
    teapot->tri_faces[108].a = 119;
    teapot->tri_faces[108].b = 117;
    teapot->tri_faces[108].c = 106;
    teapot->tri_faces[109].a = 117;
    teapot->tri_faces[109].b = 120;
    teapot->tri_faces[109].c = 118;
    teapot->tri_faces[110].a = 105;
    teapot->tri_faces[110].b = 119;
    teapot->tri_faces[110].c = 106;
    teapot->tri_faces[111].a = 105;
    teapot->tri_faces[111].b = 120;
    teapot->tri_faces[111].c = 119;
    teapot->tri_faces[112].a = 107;
    teapot->tri_faces[112].b = 122;
    teapot->tri_faces[112].c = 108;
    teapot->tri_faces[113].a = 121;
    teapot->tri_faces[113].b = 124;
    teapot->tri_faces[113].c = 122;
    teapot->tri_faces[114].a = 107;
    teapot->tri_faces[114].b = 125;
    teapot->tri_faces[114].c = 121;
    teapot->tri_faces[115].a = 121;
    teapot->tri_faces[115].b = 126;
    teapot->tri_faces[115].c = 123;
    teapot->tri_faces[116].a = 112;
    teapot->tri_faces[116].b = 125;
    teapot->tri_faces[116].c = 110;
    teapot->tri_faces[117].a = 127;
    teapot->tri_faces[117].b = 126;
    teapot->tri_faces[117].c = 125;
    teapot->tri_faces[118].a = 112;
    teapot->tri_faces[118].b = 129;
    teapot->tri_faces[118].c = 127;
    teapot->tri_faces[119].a = 127;
    teapot->tri_faces[119].b = 130;
    teapot->tri_faces[119].c = 128;
    teapot->tri_faces[120].a = 116;
    teapot->tri_faces[120].b = 129;
    teapot->tri_faces[120].c = 114;
    teapot->tri_faces[121].a = 131;
    teapot->tri_faces[121].b = 130;
    teapot->tri_faces[121].c = 129;
    teapot->tri_faces[122].a = 116;
    teapot->tri_faces[122].b = 133;
    teapot->tri_faces[122].c = 131;
    teapot->tri_faces[123].a = 131;
    teapot->tri_faces[123].b = 134;
    teapot->tri_faces[123].c = 132;
    teapot->tri_faces[124].a = 120;
    teapot->tri_faces[124].b = 133;
    teapot->tri_faces[124].c = 118;
    teapot->tri_faces[125].a = 135;
    teapot->tri_faces[125].b = 134;
    teapot->tri_faces[125].c = 133;
    teapot->tri_faces[126].a = 120;
    teapot->tri_faces[126].b = 122;
    teapot->tri_faces[126].c = 135;
    teapot->tri_faces[127].a = 135;
    teapot->tri_faces[127].b = 124;
    teapot->tri_faces[127].c = 136;
    teapot->tri_faces[128].a = 1;
    teapot->tri_faces[128].b = 2;
    teapot->tri_faces[128].c = 3;
    teapot->tri_faces[129].a = 2;
    teapot->tri_faces[129].b = 4;
    teapot->tri_faces[129].c = 5;
    teapot->tri_faces[130].a = 1;
    teapot->tri_faces[130].b = 6;
    teapot->tri_faces[130].c = 7;
    teapot->tri_faces[131].a = 2;
    teapot->tri_faces[131].b = 7;
    teapot->tri_faces[131].c = 8;
    teapot->tri_faces[132].a = 6;
    teapot->tri_faces[132].b = 9;
    teapot->tri_faces[132].c = 10;
    teapot->tri_faces[133].a = 7;
    teapot->tri_faces[133].b = 10;
    teapot->tri_faces[133].c = 11;
    teapot->tri_faces[134].a = 9;
    teapot->tri_faces[134].b = 12;
    teapot->tri_faces[134].c = 13;
    teapot->tri_faces[135].a = 10;
    teapot->tri_faces[135].b = 13;
    teapot->tri_faces[135].c = 14;
    teapot->tri_faces[136].a = 15;
    teapot->tri_faces[136].b = 16;
    teapot->tri_faces[136].c = 13;
    teapot->tri_faces[137].a = 16;
    teapot->tri_faces[137].b = 17;
    teapot->tri_faces[137].c = 14;
    teapot->tri_faces[138].a = 15;
    teapot->tri_faces[138].b = 18;
    teapot->tri_faces[138].c = 19;
    teapot->tri_faces[139].a = 16;
    teapot->tri_faces[139].b = 19;
    teapot->tri_faces[139].c = 20;
    teapot->tri_faces[140].a = 21;
    teapot->tri_faces[140].b = 22;
    teapot->tri_faces[140].c = 19;
    teapot->tri_faces[141].a = 22;
    teapot->tri_faces[141].b = 23;
    teapot->tri_faces[141].c = 20;
    teapot->tri_faces[142].a = 21;
    teapot->tri_faces[142].b = 0;
    teapot->tri_faces[142].c = 3;
    teapot->tri_faces[143].a = 22;
    teapot->tri_faces[143].b = 3;
    teapot->tri_faces[143].c = 5;
    teapot->tri_faces[144].a = 4;
    teapot->tri_faces[144].b = 24;
    teapot->tri_faces[144].c = 25;
    teapot->tri_faces[145].a = 24;
    teapot->tri_faces[145].b = 26;
    teapot->tri_faces[145].c = 27;
    teapot->tri_faces[146].a = 4;
    teapot->tri_faces[146].b = 8;
    teapot->tri_faces[146].c = 28;
    teapot->tri_faces[147].a = 24;
    teapot->tri_faces[147].b = 28;
    teapot->tri_faces[147].c = 29;
    teapot->tri_faces[148].a = 11;
    teapot->tri_faces[148].b = 30;
    teapot->tri_faces[148].c = 28;
    teapot->tri_faces[149].a = 30;
    teapot->tri_faces[149].b = 31;
    teapot->tri_faces[149].c = 29;
    teapot->tri_faces[150].a = 11;
    teapot->tri_faces[150].b = 14;
    teapot->tri_faces[150].c = 32;
    teapot->tri_faces[151].a = 30;
    teapot->tri_faces[151].b = 32;
    teapot->tri_faces[151].c = 33;
    teapot->tri_faces[152].a = 17;
    teapot->tri_faces[152].b = 34;
    teapot->tri_faces[152].c = 32;
    teapot->tri_faces[153].a = 34;
    teapot->tri_faces[153].b = 35;
    teapot->tri_faces[153].c = 33;
    teapot->tri_faces[154].a = 17;
    teapot->tri_faces[154].b = 20;
    teapot->tri_faces[154].c = 36;
    teapot->tri_faces[155].a = 34;
    teapot->tri_faces[155].b = 36;
    teapot->tri_faces[155].c = 37;
    teapot->tri_faces[156].a = 23;
    teapot->tri_faces[156].b = 38;
    teapot->tri_faces[156].c = 36;
    teapot->tri_faces[157].a = 38;
    teapot->tri_faces[157].b = 39;
    teapot->tri_faces[157].c = 37;
    teapot->tri_faces[158].a = 23;
    teapot->tri_faces[158].b = 5;
    teapot->tri_faces[158].c = 25;
    teapot->tri_faces[159].a = 38;
    teapot->tri_faces[159].b = 25;
    teapot->tri_faces[159].c = 27;
    teapot->tri_faces[160].a = 27;
    teapot->tri_faces[160].b = 26;
    teapot->tri_faces[160].c = 40;
    teapot->tri_faces[161].a = 41;
    teapot->tri_faces[161].b = 40;
    teapot->tri_faces[161].c = 42;
    teapot->tri_faces[162].a = 29;
    teapot->tri_faces[162].b = 44;
    teapot->tri_faces[162].c = 40;
    teapot->tri_faces[163].a = 44;
    teapot->tri_faces[163].b = 45;
    teapot->tri_faces[163].c = 42;
    teapot->tri_faces[164].a = 29;
    teapot->tri_faces[164].b = 31;
    teapot->tri_faces[164].c = 46;
    teapot->tri_faces[165].a = 44;
    teapot->tri_faces[165].b = 46;
    teapot->tri_faces[165].c = 47;
    teapot->tri_faces[166].a = 33;
    teapot->tri_faces[166].b = 48;
    teapot->tri_faces[166].c = 46;
    teapot->tri_faces[167].a = 48;
    teapot->tri_faces[167].b = 49;
    teapot->tri_faces[167].c = 47;
    teapot->tri_faces[168].a = 33;
    teapot->tri_faces[168].b = 35;
    teapot->tri_faces[168].c = 50;
    teapot->tri_faces[169].a = 48;
    teapot->tri_faces[169].b = 50;
    teapot->tri_faces[169].c = 51;
    teapot->tri_faces[170].a = 37;
    teapot->tri_faces[170].b = 52;
    teapot->tri_faces[170].c = 50;
    teapot->tri_faces[171].a = 52;
    teapot->tri_faces[171].b = 53;
    teapot->tri_faces[171].c = 51;
    teapot->tri_faces[172].a = 37;
    teapot->tri_faces[172].b = 39;
    teapot->tri_faces[172].c = 54;
    teapot->tri_faces[173].a = 52;
    teapot->tri_faces[173].b = 54;
    teapot->tri_faces[173].c = 55;
    teapot->tri_faces[174].a = 27;
    teapot->tri_faces[174].b = 41;
    teapot->tri_faces[174].c = 54;
    teapot->tri_faces[175].a = 41;
    teapot->tri_faces[175].b = 43;
    teapot->tri_faces[175].c = 55;
    teapot->tri_faces[176].a = 43;
    teapot->tri_faces[176].b = 42;
    teapot->tri_faces[176].c = 56;
    teapot->tri_faces[177].a = 45;
    teapot->tri_faces[177].b = 59;
    teapot->tri_faces[177].c = 56;
    teapot->tri_faces[178].a = 45;
    teapot->tri_faces[178].b = 47;
    teapot->tri_faces[178].c = 60;
    teapot->tri_faces[179].a = 49;
    teapot->tri_faces[179].b = 61;
    teapot->tri_faces[179].c = 60;
    teapot->tri_faces[180].a = 49;
    teapot->tri_faces[180].b = 51;
    teapot->tri_faces[180].c = 62;
    teapot->tri_faces[181].a = 53;
    teapot->tri_faces[181].b = 63;
    teapot->tri_faces[181].c = 62;
    teapot->tri_faces[182].a = 53;
    teapot->tri_faces[182].b = 55;
    teapot->tri_faces[182].c = 64;
    teapot->tri_faces[183].a = 43;
    teapot->tri_faces[183].b = 57;
    teapot->tri_faces[183].c = 64;
    teapot->tri_faces[184].a = 66;
    teapot->tri_faces[184].b = 67;
    teapot->tri_faces[184].c = 68;
    teapot->tri_faces[185].a = 67;
    teapot->tri_faces[185].b = 69;
    teapot->tri_faces[185].c = 70;
    teapot->tri_faces[186].a = 71;
    teapot->tri_faces[186].b = 72;
    teapot->tri_faces[186].c = 67;
    teapot->tri_faces[187].a = 72;
    teapot->tri_faces[187].b = 73;
    teapot->tri_faces[187].c = 69;
    teapot->tri_faces[188].a = 71;
    teapot->tri_faces[188].b = 74;
    teapot->tri_faces[188].c = 75;
    teapot->tri_faces[189].a = 72;
    teapot->tri_faces[189].b = 75;
    teapot->tri_faces[189].c = 76;
    teapot->tri_faces[190].a = 74;
    teapot->tri_faces[190].b = 65;
    teapot->tri_faces[190].c = 68;
    teapot->tri_faces[191].a = 75;
    teapot->tri_faces[191].b = 68;
    teapot->tri_faces[191].c = 70;
    teapot->tri_faces[192].a = 69;
    teapot->tri_faces[192].b = 77;
    teapot->tri_faces[192].c = 78;
    teapot->tri_faces[193].a = 77;
    teapot->tri_faces[193].b = 79;
    teapot->tri_faces[193].c = 33;
    teapot->tri_faces[194].a = 69;
    teapot->tri_faces[194].b = 73;
    teapot->tri_faces[194].c = 80;
    teapot->tri_faces[195].a = 80;
    teapot->tri_faces[195].b = 81;
    teapot->tri_faces[195].c = 79;
    teapot->tri_faces[196].a = 76;
    teapot->tri_faces[196].b = 82;
    teapot->tri_faces[196].c = 80;
    teapot->tri_faces[197].a = 80;
    teapot->tri_faces[197].b = 82;
    teapot->tri_faces[197].c = 83;
    teapot->tri_faces[198].a = 76;
    teapot->tri_faces[198].b = 70;
    teapot->tri_faces[198].c = 78;
    teapot->tri_faces[199].a = 82;
    teapot->tri_faces[199].b = 78;
    teapot->tri_faces[199].c = 33;
    teapot->tri_faces[200].a = 84;
    teapot->tri_faces[200].b = 85;
    teapot->tri_faces[200].c = 86;
    teapot->tri_faces[201].a = 86;
    teapot->tri_faces[201].b = 88;
    teapot->tri_faces[201].c = 89;
    teapot->tri_faces[202].a = 85;
    teapot->tri_faces[202].b = 90;
    teapot->tri_faces[202].c = 91;
    teapot->tri_faces[203].a = 91;
    teapot->tri_faces[203].b = 92;
    teapot->tri_faces[203].c = 88;
    teapot->tri_faces[204].a = 93;
    teapot->tri_faces[204].b = 94;
    teapot->tri_faces[204].c = 91;
    teapot->tri_faces[205].a = 91;
    teapot->tri_faces[205].b = 94;
    teapot->tri_faces[205].c = 95;
    teapot->tri_faces[206].a = 84;
    teapot->tri_faces[206].b = 87;
    teapot->tri_faces[206].c = 94;
    teapot->tri_faces[207].a = 94;
    teapot->tri_faces[207].b = 87;
    teapot->tri_faces[207].c = 89;
    teapot->tri_faces[208].a = 88;
    teapot->tri_faces[208].b = 96;
    teapot->tri_faces[208].c = 97;
    teapot->tri_faces[209].a = 97;
    teapot->tri_faces[209].b = 96;
    teapot->tri_faces[209].c = 98;
    teapot->tri_faces[210].a = 92;
    teapot->tri_faces[210].b = 100;
    teapot->tri_faces[210].c = 96;
    teapot->tri_faces[211].a = 96;
    teapot->tri_faces[211].b = 100;
    teapot->tri_faces[211].c = 101;
    teapot->tri_faces[212].a = 92;
    teapot->tri_faces[212].b = 95;
    teapot->tri_faces[212].c = 102;
    teapot->tri_faces[213].a = 102;
    teapot->tri_faces[213].b = 103;
    teapot->tri_faces[213].c = 101;
    teapot->tri_faces[214].a = 95;
    teapot->tri_faces[214].b = 89;
    teapot->tri_faces[214].c = 97;
    teapot->tri_faces[215].a = 97;
    teapot->tri_faces[215].b = 99;
    teapot->tri_faces[215].c = 103;
    teapot->tri_faces[216].a = 105;
    teapot->tri_faces[216].b = 104;
    teapot->tri_faces[216].c = 107;
    teapot->tri_faces[217].a = 109;
    teapot->tri_faces[217].b = 110;
    teapot->tri_faces[217].c = 107;
    teapot->tri_faces[218].a = 109;
    teapot->tri_faces[218].b = 111;
    teapot->tri_faces[218].c = 112;
    teapot->tri_faces[219].a = 113;
    teapot->tri_faces[219].b = 114;
    teapot->tri_faces[219].c = 112;
    teapot->tri_faces[220].a = 113;
    teapot->tri_faces[220].b = 115;
    teapot->tri_faces[220].c = 116;
    teapot->tri_faces[221].a = 117;
    teapot->tri_faces[221].b = 118;
    teapot->tri_faces[221].c = 116;
    teapot->tri_faces[222].a = 117;
    teapot->tri_faces[222].b = 119;
    teapot->tri_faces[222].c = 120;
    teapot->tri_faces[223].a = 105;
    teapot->tri_faces[223].b = 108;
    teapot->tri_faces[223].c = 120;
    teapot->tri_faces[224].a = 107;
    teapot->tri_faces[224].b = 121;
    teapot->tri_faces[224].c = 122;
    teapot->tri_faces[225].a = 121;
    teapot->tri_faces[225].b = 123;
    teapot->tri_faces[225].c = 124;
    teapot->tri_faces[226].a = 107;
    teapot->tri_faces[226].b = 110;
    teapot->tri_faces[226].c = 125;
    teapot->tri_faces[227].a = 121;
    teapot->tri_faces[227].b = 125;
    teapot->tri_faces[227].c = 126;
    teapot->tri_faces[228].a = 112;
    teapot->tri_faces[228].b = 127;
    teapot->tri_faces[228].c = 125;
    teapot->tri_faces[229].a = 127;
    teapot->tri_faces[229].b = 128;
    teapot->tri_faces[229].c = 126;
    teapot->tri_faces[230].a = 112;
    teapot->tri_faces[230].b = 114;
    teapot->tri_faces[230].c = 129;
    teapot->tri_faces[231].a = 127;
    teapot->tri_faces[231].b = 129;
    teapot->tri_faces[231].c = 130;
    teapot->tri_faces[232].a = 116;
    teapot->tri_faces[232].b = 131;
    teapot->tri_faces[232].c = 129;
    teapot->tri_faces[233].a = 131;
    teapot->tri_faces[233].b = 132;
    teapot->tri_faces[233].c = 130;
    teapot->tri_faces[234].a = 116;
    teapot->tri_faces[234].b = 118;
    teapot->tri_faces[234].c = 133;
    teapot->tri_faces[235].a = 131;
    teapot->tri_faces[235].b = 133;
    teapot->tri_faces[235].c = 134;
    teapot->tri_faces[236].a = 120;
    teapot->tri_faces[236].b = 135;
    teapot->tri_faces[236].c = 133;
    teapot->tri_faces[237].a = 135;
    teapot->tri_faces[237].b = 136;
    teapot->tri_faces[237].c = 134;
    teapot->tri_faces[238].a = 120;
    teapot->tri_faces[238].b = 108;
    teapot->tri_faces[238].c = 122;
    teapot->tri_faces[239].a = 135;
    teapot->tri_faces[239].b = 122;
    teapot->tri_faces[239].c = 124;
}


void simplified_perspective_teapot(pixel_buffer* p_buffer, double delta_time, double total_time) {

    model teapot;
    double pi = 3.14159265358979323846f;
    // float deg90 = 3.14159265358979323846f/2.0f;

    //larger is slower
    double x_speed_factor = 20.0;
    double y_speed_factor = 10.0;
    double z_speed_factor = 40.0;
    // turn total time into 0..1 interval
    double x_value = (total_time - ((int) (total_time/x_speed_factor))*x_speed_factor)/x_speed_factor;
    double y_value = (total_time - ((int) (total_time/y_speed_factor))*y_speed_factor)/y_speed_factor;
    double z_value = (total_time - ((int) (total_time/z_speed_factor))*z_speed_factor)/z_speed_factor;
    // turn 0..1 into 0..2pi
    x_value = x_value*2.0*pi;
    y_value = y_value*2.0*pi;
    z_value = z_value*2.0*pi;

    create_teapot(&teapot);

    reset_model_transform(&teapot);
    rotate_model(&teapot, x_value, y_value, z_value);
    // rotate_model(&cube, -x_value, 0.0f, 0.0f);
    // rotate_model(&cube, 0.0f, 0.0f, -z_value);
    
    // scale_model(&teapot, 0.25f, 0.25f, 0.25f);
    translate_model(&teapot, 0.0f, 0.0f, 1.0f);

    draw_model_wireframe(p_buffer, &teapot, simplified_perspective_projection, 0x00FF00FF);
}

void rasterization_scanlines_teapot(pixel_buffer* p_buffer, double delta_time, double total_time) {

    srand(time(NULL));

    model teapot;
    double pi = 3.14159265358979323846f;
    // float deg90 = 3.14159265358979323846f/2.0f;

    //larger is slower
    double x_speed_factor = 20.0;
    double y_speed_factor = 10.0;
    double z_speed_factor = 40.0;
    // turn total time into 0..1 interval
    double x_value = (total_time - ((int) (total_time/x_speed_factor))*x_speed_factor)/x_speed_factor;
    double y_value = (total_time - ((int) (total_time/y_speed_factor))*y_speed_factor)/y_speed_factor;
    double z_value = (total_time - ((int) (total_time/z_speed_factor))*z_speed_factor)/z_speed_factor;
    // turn 0..1 into 0..2pi
    x_value = x_value*2.0*pi;
    y_value = y_value*2.0*pi;
    z_value = z_value*2.0*pi;

    create_teapot(&teapot);

    reset_model_transform(&teapot);
    rotate_model(&teapot, x_value, y_value, z_value);
    // rotate_model(&cube, -x_value, 0.0f, 0.0f);
    // rotate_model(&cube, 0.0f, 0.0f, -z_value);
    
    scale_model(&teapot, 0.75f, 0.75f, 0.75f);
    translate_model(&teapot, 0.0f, 0.0f, -1.0f);

    draw_model_scanlines(p_buffer, &teapot, simplified_perspective_projection, 0x0000FFFF);
    // draw_model_wireframe(p_buffer, &teapot, simplified_perspective_projection, 0xFFFFFFFF);

    free_model_data(&teapot);
}

void rasterization_edge_functions_teapot(pixel_buffer* p_buffer, double delta_time, double total_time) {

    srand(time(NULL));

    model my_model;
    double pi = 3.14159265358979323846f;
    // float deg90 = 3.14159265358979323846f/2.0f;

    //larger is slower
    double x_speed_factor = 20.0;
    double y_speed_factor = 10.0;
    double z_speed_factor = 40.0;
    // turn total time into 0..1 interval
    double x_value = (total_time - ((int) (total_time/x_speed_factor))*x_speed_factor)/x_speed_factor;
    double y_value = (total_time - ((int) (total_time/y_speed_factor))*y_speed_factor)/y_speed_factor;
    double z_value = (total_time - ((int) (total_time/z_speed_factor))*z_speed_factor)/z_speed_factor;
    // turn 0..1 into 0..2pi
    x_value = x_value*2.0*pi;
    y_value = y_value*2.0*pi;
    z_value = z_value*2.0*pi;

    create_teapot(&my_model);
    // create_test_cube(&my_model);
    // create_test_triangle(&my_model);

    reset_model_transform(&my_model);
    rotate_model(&my_model, x_value, y_value, z_value);
    // rotate_model(&cube, -x_value, 0.0f, 0.0f);
    // rotate_model(&cube, 0.0f, 0.0f, -z_value);
    
    scale_model(&my_model, 0.75f, 0.75f, 0.75f);
    translate_model(&my_model, 0.0f, 0.0f, -1.0f);

    draw_model_edge_functions(p_buffer, &my_model, simplified_perspective_projection, 0x0000FFFF);
    // draw_model_wireframe(p_buffer, &teapot, simplified_perspective_projection, 0xFFFFFFFF);

    free_model_data(&my_model);

    // --- new test ---

    // static double last_time = -1.0f;

    // static float x1 = 300.0f;
    // static float y1 = 300.0f;
    // static float x2 = 500.0f;
    // static float y2 = 300.0f;
    // static float x3 = 400.0f;
    // static float y3 = 500.0f;

    // double interval = 0.25f;

    // if (total_time > last_time + interval) {
    //     last_time = total_time;

    //     x1 = rand()%400 - 200.0f + (p_buffer->width/2.0f);
    //     x2 = rand()%400 - 200.0f + (p_buffer->width/2.0f);
    //     x3 = rand()%400 - 200.0f + (p_buffer->width/2.0f);
    //     y1 = rand()%400 - 200.0f + (p_buffer->height/2.0f);
    //     y2 = rand()%400 - 200.0f + (p_buffer->height/2.0f);
    //     y3 = rand()%400 - 200.0f + (p_buffer->height/2.0f);
    // }

    // rasterize_triangle_edge_functions(p_buffer, x1, y1, x2, y2, x3, y3, 0xFF0000FF);
}