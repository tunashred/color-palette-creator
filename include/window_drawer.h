#ifndef WINDOW_DRAWER_H
#define WINDOW_DRAWER_H

#include "color_functions.h"
#include "palette_manager.h"
#include "sdl_manager.h"

#include <SDL2/SDL.h>


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

void update_and_render_texture(SDL_Renderer* renderer, SDL_Texture* texture, color_palette* palette, int window_width, int window_height);

#endif
