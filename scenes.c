#include "scenes.h"

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
                draw_pixel_safe(p_buffer, x, y, red);
            }
            else 
            {
                draw_pixel_safe(p_buffer, x, y, black);
            }
        }
        
    }
}