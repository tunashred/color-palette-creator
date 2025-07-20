#ifndef REDRAW_DATA_H
#define REDRAW_DATA_H

#include <stdbool.h>
#include <SDL2/SDL.h>

typedef uint8_t (*color_mapping_func)(int, int);

enum Mode {MANDELBROT_SINGLECORE = 1, PALETTE};

typedef struct {
    SDL_Renderer* renderer;
    SDL_Texture* texture;
    void (*p_func_draw_to_texture)(void*);
    void* args;
    SDL_mutex* redraw_mutex;
    SDL_cond* redraw_cond;
    int window_width;
    int window_height;
    bool running;
    bool currently_drawing;
    bool render_ready;
    volatile bool cancel_current_draw;
    int mode;
} redraw_event_data;

#endif