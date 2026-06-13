#include <SDL3/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "graphics_module.h"
#include "custom_scenes.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define BACKGROUND_COLOR 0x202020ff

typedef struct sdl_pointers {
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* texture;
} sdl_pointers;

// timing variables for FPS, frametime and keeping pacing of animations
typedef struct time_state {
    uint64_t last_time;
    uint64_t frequency;
    double fps_timer;
    int frame_count;
    uint64_t current_time;
    double delta_time; 
    double avg_fps;
    double ms_per_frame;
    bool one_second_flag;
    double total_time;
} time_state;

bool populate_sdl_pointers(sdl_pointers* sdlp) {

    sdlp->window = SDL_CreateWindow("C Graphics", SCREEN_WIDTH, SCREEN_HEIGHT, 0x00);
    if (!sdlp->window) {
        SDL_Log("Could not create window: %s", SDL_GetError());
        SDL_Quit();
        return false;
    }

    sdlp->renderer = SDL_CreateRenderer(sdlp->window, NULL);
    if (!sdlp->renderer) {
        SDL_Log("Could not create renderer: %s", SDL_GetError());
        SDL_Quit();
        return false;
    }

    // turn on vsync to synchronize present with every vertical refresh
    SDL_SetRenderVSync(sdlp->renderer, 1);

    // Create a streaming texture matching our screen dimensions
    sdlp->texture = SDL_CreateTexture(
        sdlp->renderer, 
        SDL_PIXELFORMAT_RGBA8888, 
        SDL_TEXTUREACCESS_STREAMING, 
        SCREEN_WIDTH, 
        SCREEN_HEIGHT
    );
    if (!sdlp->texture) {
        SDL_Log("Could not create texture: %s", SDL_GetError());
        SDL_Quit();
        return false;
    }

    return true;
}

bool sdl_setup(sdl_pointers* sdlp) {

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
        SDL_Quit();
        return false;
    }

    return populate_sdl_pointers(sdlp);
}

void setup_time_state(time_state* t_state) {

    t_state->last_time = SDL_GetPerformanceCounter();
    t_state->frequency = SDL_GetPerformanceFrequency();
    t_state->fps_timer = 0.0;
    t_state->frame_count = 0;
    t_state->one_second_flag = false;
    t_state->total_time = 0.0;
}

void update_time_state(time_state* t_state) {

    // calculate delta time (seconds passed since last frame)
    t_state->current_time = SDL_GetPerformanceCounter();
    t_state->delta_time = (double)(t_state->current_time - t_state->last_time) / (double)t_state->frequency;
    t_state->last_time = t_state->current_time;

    // accumulate time for total time
    t_state->total_time += t_state->delta_time;

    // accumulate time for the FPS display
    t_state->fps_timer += t_state->delta_time;
    t_state->frame_count++;

    // update avg fps and ms per frame once every second
    if (t_state->fps_timer >= 1.0) {
        t_state->avg_fps = (double)t_state->frame_count / t_state->fps_timer;
        t_state->ms_per_frame = (t_state->fps_timer / (double)t_state->frame_count) * 1000.0;

        // reset sampling buckets
        t_state->fps_timer = 0.0;
        t_state->frame_count = 0;

        // set flag for fps update
        t_state->one_second_flag = true;
    }
}

bool update_title(sdl_pointers* sdlp, char* title_buffer, size_t size, time_state* t_state) {

    snprintf(title_buffer, size, 
        "C Graphics | FPS: %.1f | %.2f ms/frame", 
        t_state->avg_fps, t_state->ms_per_frame);
        
    if(!SDL_SetWindowTitle(sdlp->window, title_buffer)) {
        SDL_Log("Could not set window title to %s: %s", title_buffer, SDL_GetError());
        SDL_Quit();
        return false;
    }

    return true;
}

void clear_frame(pixel_buffer* p_buffer) {

    clear_color_buffer(p_buffer, 0x202020FF);
}

void draw_frame(pixel_buffer* p_buffer, double delta_time, double total_time) {

    // call scene function
    // checkered_board_animation(p_buffer, delta_time, total_time);
    // lines_dda_triangle(p_buffer, delta_time, total_time);
    lines_bresenham_all_octants(p_buffer, delta_time, total_time);

}

bool present_frame(sdl_pointers* sdlp, pixel_buffer* p_buffer) {

    // Copy our raw C buffer into the SDL Texture memory
    void* texture_pixels;
    int pitch;
    if (SDL_LockTexture(sdlp->texture, NULL, &texture_pixels, &pitch)) {
        // Copy all at once since memory is contiguous
        memcpy(texture_pixels, p_buffer->color_buffer, p_buffer->width * p_buffer->height * sizeof(uint32_t));
        SDL_UnlockTexture(sdlp->texture);
    }
    else
    {
        SDL_Log("Could not lock texture: %s", SDL_GetError());
        SDL_Quit();
        return false;
    }

    // Blit the texture to the screen
    SDL_RenderClear(sdlp->renderer);
    SDL_RenderTexture(sdlp->renderer, sdlp->texture, NULL, NULL);
    SDL_RenderPresent(sdlp->renderer);

    return true;
}

bool frame_loop(sdl_pointers* sdlp, pixel_buffer* p_buffer, time_state* t_state) {

    // --- PHASE 1: CLEAR THE CANVAS ---
    clear_frame(p_buffer);

    // --- PHASE 2: GRAPHICS ALGORITHMS ---
    draw_frame(p_buffer, t_state->delta_time, t_state->total_time);

    // --- PHASE 3: PRESENTATION ---
    return present_frame(sdlp, p_buffer);
}

void clean_up_before_exit(sdl_pointers* sdlp, pixel_buffer* p_buffer) {

    // Free resources
    free(p_buffer->color_buffer);
    SDL_DestroyTexture(sdlp->texture);
    SDL_DestroyRenderer(sdlp->renderer);
    SDL_DestroyWindow(sdlp->window);
    SDL_Quit();
}

int main(int argc, char* argv[]) {

    sdl_pointers sdlp;
    pixel_buffer p_buffer;
    time_state t_state;
    char title_buffer[128];

    if (!sdl_setup(&sdlp)) {
        return 1;
    }

    if (!setup_pixel_buffer(&p_buffer, SCREEN_WIDTH, SCREEN_HEIGHT)) {
        SDL_Log("Could not allocate memory for color buffer.\n");
        SDL_Quit();
        return 1;
    }

    // type used to handle sdl events
    SDL_Event event;
    // main loop will stop once running is false
    bool running = true;

    setup_time_state(&t_state);

    // main loop
    while (running) {

        update_time_state(&t_state);

        if (t_state.one_second_flag) {
            if (!update_title(&sdlp, title_buffer, sizeof(title_buffer), &t_state)) {
                return 1;
            }

            // printf("\nFLAG!\n");

            t_state.one_second_flag = false;
        }

        // Handle Inputs
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) running = 0;
        }

        frame_loop(&sdlp, &p_buffer, &t_state);
    }

    clean_up_before_exit(&sdlp, &p_buffer);

    return 0;
}