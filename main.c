#include <SDL3/SDL.h>
#include <stdio.h>

int main(int argc, char* argv[]) {
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("Software Rasterizer", 800, 600, 0);
    if (!window) {
        SDL_Log("Could not create window: %s", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, NULL);
    
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
            
            SDL_SetWindowTitle(window, title_buffer);

            // Reset sampling buckets
            fps_timer = 0.0;
            frame_count = 0;
        }

        // Handle Inputs
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                running = 0;
            }
        }

        // --- YOUR RENDERER LOGIC ---
        SDL_SetRenderDrawColor(renderer, 32, 32, 32, 255);
        SDL_RenderClear(renderer);
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}