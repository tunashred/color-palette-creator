#ifndef PALETTE_MANAGER_H
#define PALETTE_MANAGER_H

#include <stdint.h>
#include <SDL2/SDL.h>

#include "common/common.h"
#include "common/color_functions.h"

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
                            color_mapping_func r_func, color_mapping_func g_func, 
                            color_mapping_func b_func);

void export_palette(color_palette* palette);

void color_function_changer(int pressed_key, color_palette* palette);

void draw_palette_to_texture(void* args);

#endif
