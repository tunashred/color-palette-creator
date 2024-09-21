#ifndef WINDOW_DRAWER_H
#define WINDOW_DRAWER_H

#include "color_mapping.h"

#include <SDL2/SDL.h>


SDL_Window* window_init();

SDL_Renderer* create_renderer(SDL_Window* window, int index, Uint32 flags);

void draw_palette(SDL_Renderer* renderer, int window_width, int window_height, 
                  uint8_t* red_array, uint8_t* green_array, uint8_t* blue_array, uint8_t (*combined_array)[3]);

typedef struct {
    SDL_Renderer* renderer;
    color_palette* palette;
    int window_width;
    int window_height;
} timer_data;

Uint32 timer_callback(Uint32 interval, void* args);

#endif