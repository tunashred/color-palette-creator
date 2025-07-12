#ifndef REDRAW_DATA_H
#define REDRAW_DATA_H

#include <SDL2/SDL.h>

typedef struct {
    SDL_Renderer* renderer;
    SDL_Texture* texture;
    void (*p_func_draw_to_texture)(void*);
    void* args;
    int window_width;
    int window_height;
} redraw_event_data;

enum Mode {MANDELBROT = 1, PALETTE};

#endif