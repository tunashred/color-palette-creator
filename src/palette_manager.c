#include "palette_manager.h"

void generate_color_palette(
        color_palette* palette,
        char* palette_file,
        double brightness_rate,
        uint8_t (*red_func)(int, int),
        uint8_t (*green_func)(int, int),
        uint8_t (*blue_func)(int, int)
) {
    // Case 1: Loading from file
    if(palette_file) {
        FILE* file = fopen(palette_file, "r");
        if(!file) {
            fprintf(stderr, "Couldn't open the file: %s\n", palette_file);
            exit(EXIT_FAILURE);
        }
        
        for(int i = 0; i < NUM_COLORS; i++) {
            if(fscanf(file, "%hhu %hhu %hhu", &palette->r[i], &palette->g[i], &palette->b[i]) != RGB_CHANNELS) {
                fprintf(stderr, "Incomplete palette file\n");
                exit(EXIT_FAILURE);
            }
            palette->rgb[i][0] = (uint8_t) fmin(1499, (double)palette->r[i] * brightness_rate);
            palette->rgb[i][1] = (uint8_t) fmin(1499, (double)palette->g[i] * brightness_rate);
            palette->rgb[i][2] = (uint8_t) fmin(1499, (double)palette->b[i] * brightness_rate);
        }
        fclose(file);
        return;
    }
    if (!red_func || !green_func || !blue_func) {
        fprintf(stderr, "Please provide all color mapping functions\n");
        exit(EXIT_FAILURE);
    }
    // Case 2: Generate using given color mapping functions
    palette->red_func = red_func;
    palette->green_func = green_func;
    palette->blue_func = blue_func;
    for(int i = 0; i < NUM_COLORS; i++) {
        palette->r[i] = map_color(i, NUM_COLORS, red_func);
        palette->g[i] = map_color(i, NUM_COLORS, green_func);
        palette->b[i] = map_color(i, NUM_COLORS, blue_func);

        palette->rgb[i][0] = (uint8_t) fmin(1499, (double)palette->r[i] * brightness_rate);
        palette->rgb[i][1] = (uint8_t) fmin(1499, (double)palette->g[i] * brightness_rate);
        palette->rgb[i][2] = (uint8_t) fmin(1499, (double)palette->b[i] * brightness_rate);
    }
}

void color_function_changer(int pressed_key, color_palette* palette) {
    static uint8_t (*color_mapping_functions[NUM_MAPPING_FUNCTIONS]) (int, int) = {
        sin_crescator, log_pe_sin, unu_minus_unu_pe_x, x_patrat_0_5, x_patrat_0_1,
        x_patrat_0_1_to_0_2, sin_jos_sus_jos, sin_x_la_4, mapare_simpla
    };
    static int r_index = 0, g_index = 0, b_index = 0;
    
    switch(pressed_key) {
        case SDLK_r:
            if(r_index >= NUM_MAPPING_FUNCTIONS) {
                r_index = 0;
            }
            generate_color_palette(palette, NULL, 1, color_mapping_functions[r_index++], palette->green_func, palette->blue_func);
            break;

        case SDLK_g:
            if(g_index >= NUM_MAPPING_FUNCTIONS) {
                g_index = 0;
            }
            generate_color_palette(palette, NULL, 1, palette->red_func, color_mapping_functions[g_index++], palette->blue_func);
            break;

        case SDLK_b:
            if(b_index >= NUM_MAPPING_FUNCTIONS) {
                b_index = 0;
            }
            generate_color_palette(palette, NULL, 1, palette->red_func, palette->green_func, color_mapping_functions[b_index++]);
            break;

        default: break;
    }
}
