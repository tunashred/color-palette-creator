#ifndef WINDOW_DRAWER_H
#define WINDOW_DRAWER_H

#include "color_mapping.h"

#include <SDL2/SDL.h>


SDL_Window* window_init();

SDL_Renderer* create_renderer(SDL_Window* window, int index, Uint32 flags);

void draw_palette_to_texture(SDL_Texture* texture, int window_width,
                            int window_height, uint8_t* red_array, uint8_t* green_array,
                            uint8_t* blue_array, uint8_t (*combined_array)[3]);

typedef struct {
    SDL_Renderer* renderer;
    color_palette* palette;
    int window_width;
    int window_height;
    SDL_Texture* texture;
} redraw_event_data;

Uint32 trigger_redraw_event(Uint32 interval, void* args);

void color_function_changer(int pressed_key, color_palette* palette);

void create_palette(
    char* file_name, double brightness_rate, uint8_t (*red_func)(int, int),
    uint8_t (*green_func)(int, int), uint8_t (*blue_func)(int, int)
);

#endif
