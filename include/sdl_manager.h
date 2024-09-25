#ifndef SDL_MANAGER_H
#define SDL_MANAGER_H

#include <SDL2/SDL.h>

SDL_Window* window_init();

SDL_Renderer* create_renderer(SDL_Window* window, int index, Uint32 flags);

Uint32 scheduled_redraw(Uint32 interval, void* args);

void render_palette_window(
    char* file_name, double brightness_rate, uint8_t (*red_func)(int, int),
    uint8_t (*green_func)(int, int), uint8_t (*blue_func)(int, int)
);

#endif
