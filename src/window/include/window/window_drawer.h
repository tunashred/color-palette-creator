#ifndef WINDOW_DRAWER_H
#define WINDOW_DRAWER_H

#include <SDL2/SDL.h>

#include "common/common.h"

void draw(redraw_event_data* data);

int draw_worker(void* args);

void render_mandelbrot(redraw_event_data* args);

#endif
