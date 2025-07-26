#ifndef SDL_MANAGER_H
#define SDL_MANAGER_H

#include <SDL2/SDL.h>

#include "common/common.h"

SDL_Window* window_init(int window_width, int window_height);

SDL_Renderer* create_renderer(SDL_Window* window, int index, uint32_t flags);

SDL_Texture* create_texture(SDL_Renderer* renderer, uint32_t format, int access, int width, int height);

void handle_sdl_events(SDL_Event* event, SDL_Window* window, redraw_event_data* redraw_info, int mode);

// for uploading a file I guess
void render_palette_window(char* file_name, double brightness_rate, color_mapping_func red_func,
                           color_mapping_func green_func, color_mapping_func blue_func);

#endif
