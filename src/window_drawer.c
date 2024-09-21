#include "window_drawer.h"

SDL_Window* window_init() {
    if(SDL_InitSubSystem(SDL_INIT_VIDEO | SDL_INIT_TIMER)) {
        printf("SDL_InitSubSystem error: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }
    
    SDL_Window* window = SDL_CreateWindow(
                                            "Color palette creator", SDL_WINDOWPOS_CENTERED,
                                            SDL_WINDOWPOS_CENTERED, 640, 480,
                                            SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE
                                        );

    if(!window) {
        printf("SDL_CreateWindow error: %s\n", SDL_GetError());
        SDL_Quit();
        exit(EXIT_FAILURE);
    }

    return window;
}

SDL_Renderer* create_renderer(SDL_Window* window, int index, Uint32 flags) {
    SDL_Renderer* renderer = SDL_CreateRenderer(window, index, flags);
    if(!renderer) {
        SDL_DestroyWindow(window);
        printf("SDL_CreateRenderer error: %s\n", SDL_GetError());
        SDL_Quit();
        exit(EXIT_FAILURE);
    }
    return renderer;
}

void draw_palette(SDL_Renderer* renderer, int window_width, int window_height, 
                  uint8_t* red_array, uint8_t* green_array, uint8_t* blue_array, uint8_t (*combined_array)[3]) {
    
    int stripe_height = window_height / 4;

    for (int y = 0; y < stripe_height; ++y) {
        for (int x = 0; x < window_width; ++x) {
            uint8_t red = red_array[x % window_width];
            SDL_SetRenderDrawColor(renderer, red, 0, 0, 255);
            SDL_RenderDrawPoint(renderer, x, y);
        }
    }

    for (int y = stripe_height; y < 2 * stripe_height; ++y) {
        for (int x = 0; x < window_width; ++x) {
            uint8_t green = green_array[x % window_width];
            SDL_SetRenderDrawColor(renderer, 0, green, 0, 255);
            SDL_RenderDrawPoint(renderer, x, y);
        }
    }

    // Draw the blue stripe
    for (int y = 2 * stripe_height; y < 3 * stripe_height; ++y) {
        for (int x = 0; x < window_width; ++x) {
            uint8_t blue = blue_array[x % window_width];
            SDL_SetRenderDrawColor(renderer, 0, 0, blue, 255);
            SDL_RenderDrawPoint(renderer, x, y);
        }
    }

    // Draw the combined RGB stripe
    for (int y = 3 * stripe_height; y < 4 * stripe_height; ++y) {
        for (int x = 0; x < window_width; ++x) {
            uint8_t red = combined_array[x % window_width][0];
            uint8_t green = combined_array[x % window_width][1];
            uint8_t blue = combined_array[x % window_width][2];
            SDL_SetRenderDrawColor(renderer, red, green, blue, 255);
            SDL_RenderDrawPoint(renderer, x, y);
        }
    }
    SDL_RenderPresent(renderer);
}

Uint32 timer_callback(Uint32 interval, void* args) {
    timer_data* timer_info = (timer_data*) args;
    SDL_SetRenderDrawColor(timer_info->renderer, 0, 0, 0, 255);
    SDL_RenderClear(timer_info->renderer);
    
    draw_palette(timer_info->renderer, timer_info->window_width, timer_info->window_height, timer_info->palette->r, timer_info->palette->g, timer_info->palette->b, timer_info->palette->rgb);

    SDL_RenderPresent(timer_info->renderer);

    return interval;
}
