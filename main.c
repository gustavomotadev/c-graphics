#include <SDL3/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

// Our custom pixel buffer (The Canvas)
uint32_t* color_buffer = NULL;

// Helper function to clear the buffer with a single color
void clear_color_buffer(uint32_t color) {
    for (int i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; i++) {
        color_buffer[i] = color;
    }
}

// Helper function to draw a single point
void draw_pixel_safe(int x, int y, uint32_t color) {
    // Prevent out-of-bounds memory writes (bounds checking)
    if (x >= 0 && x < SCREEN_WIDTH && y >= 0 && y < SCREEN_HEIGHT) {
        color_buffer[(SCREEN_HEIGHT - 1 - y) * SCREEN_WIDTH + x] = color; 
        // Note: (SCREEN_HEIGHT - 1 - y) flips the Y axis so (0,0) is at the bottom-left,
        // which is standard for 3D graphics math.
    }
}

int main(int argc, char* argv[]) {

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("C Graphics", SCREEN_WIDTH, SCREEN_HEIGHT, 0x00);
    if (!window) {
        SDL_Log("Could not create window: %s", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, NULL);
    if (!window) {
        SDL_Log("Could not create renderer: %s", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    // 1. Create a streaming texture matching our screen dimensions
    SDL_Texture* ready_texture = SDL_CreateTexture(
        renderer, 
        SDL_PIXELFORMAT_RGBA8888, 
        SDL_TEXTUREACCESS_STREAMING, 
        SCREEN_WIDTH, 
        SCREEN_HEIGHT
    );

    // 2. Allocate our raw C array on the heap
    color_buffer = (uint32_t*)malloc(SCREEN_WIDTH * SCREEN_HEIGHT * sizeof(uint32_t));
    if (!color_buffer) {
        SDL_Log("Could not allocate memory for color buffer.\n");
        SDL_Quit();
        return 1;
    }

    SDL_Event event;
    int running = 1;

    // Timing Variables
    uint64_t last_time = SDL_GetPerformanceCounter();
    uint64_t frequency = SDL_GetPerformanceFrequency();
    
    double fps_timer = 0.0;
    int frame_count = 0;

    // App Loop
    while (running) {

        // 1. Calculate Delta Time (seconds passed since last frame)
        uint64_t current_time = SDL_GetPerformanceCounter();
        double delta_time = (double)(current_time - last_time) / (double)frequency;
        last_time = current_time;

        // 2. Accumulate time for the FPS display
        fps_timer += delta_time;
        frame_count++;

        // 3. Update window title exactly once every second
        if (fps_timer >= 1.0) {
            double avg_fps = (double)frame_count / fps_timer;
            double ms_per_frame = (fps_timer / (double)frame_count) * 1000.0;
            
            char title_buffer[128];
            snprintf(title_buffer, sizeof(title_buffer), 
                     "C Graphics | FPS: %.1f | %.2f ms/frame", 
                     avg_fps, ms_per_frame);
            
            if(!SDL_SetWindowTitle(window, title_buffer)) {
                SDL_Log("Could not set window title to %s: %s", title_buffer, SDL_GetError());
                SDL_Quit();
                return 1;
            }

            // Reset sampling buckets
            fps_timer = 0.0;
            frame_count = 0;
        }

        // Handle Inputs
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) running = 0;
        }

        // --- PHASE 1: CLEAR THE CANVAS ---
        clear_color_buffer(0x202020FF); // Dark blue-gray

        // --- PHASE 2: YOUR GRAPHICS ALGORITHMS (REINVENTING THE WHEEL) ---
        // For now, let's just draw a single yellow point in the center of the screen
        draw_pixel_safe(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0xFFFF00FF);

        // --- PHASE 3: PRESENTATION ---
        // Copy our raw C buffer into the SDL Texture memory
        void* texture_pixels;
        int pitch;
        if (SDL_LockTexture(ready_texture, NULL, &texture_pixels, &pitch)) {
            // Copy row by row (or all at once since memory is contiguous)
            memcpy(texture_pixels, color_buffer, SCREEN_WIDTH * SCREEN_HEIGHT * sizeof(uint32_t));
            SDL_UnlockTexture(ready_texture);
        }
        else
        {
            SDL_Log("Could not lock texture: %s", SDL_GetError());
            SDL_Quit();
            return 1;
        }

        // Blit the texture to the screen
        SDL_RenderClear(renderer);
        SDL_RenderTexture(renderer, ready_texture, NULL, NULL);
        SDL_RenderPresent(renderer);
    }

    // Free resources
    free(color_buffer);
    SDL_DestroyTexture(ready_texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}