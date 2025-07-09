#ifndef WINDOW_DRAWER_H
#define WINDOW_DRAWER_H

#include "color_functions.h"
#include "palette_manager.h"
#include "sdl_manager.h"

#include <SDL2/SDL.h>

typedef struct {
    SDL_Renderer* renderer;
    SDL_Texture* texture;
    void* buffer_data;
    void (*p_func_draw_to_texture)(void*);
    int window_width;
    int window_height;
} redraw_event_data;

void update_and_render_texture(redraw_event_data* data);

#endif
