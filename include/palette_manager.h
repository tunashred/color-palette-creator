#ifndef PALETTE_MANAGER_H
#define PALETTE_MANAGER_H

#include "color_functions.h"

#include <stdint.h>
#include <SDL2/SDL.h>

typedef struct {
    uint8_t r[NUM_COLORS];
    uint8_t g[NUM_COLORS];
    uint8_t b[NUM_COLORS];
    uint8_t rgb[NUM_COLORS][3];
    uint8_t (*red_func)(int, int);
    uint8_t (*green_func)(int, int);
    uint8_t (*blue_func)(int, int);
} color_palette;

void generate_color_palette(color_palette* palette, char* file_name, double brightness_rate, 
                            uint8_t (*r_func)(int, int), uint8_t (*g_func)(int, int), 
                            uint8_t (*b_func)(int, int));

void color_function_changer(int pressed_key, color_palette* palette);

#endif
