#ifndef REDRAW_DATA_H
#define REDRAW_DATA_H

#include <SDL2/SDL.h>

typedef uint8_t (*color_mapping_func)(int, int);

enum Mode {MANDELBROT_SINGLECORE = 1, PALETTE};

typedef uint32_t* (*mandelbrot_singlecore_func)(
                                                const char*, int, int, double, double, double, int, double,
                                                double, color_mapping_func, color_mapping_func, color_mapping_func);

typedef struct {
    SDL_Renderer* renderer;
    SDL_Texture* texture;
    void (*p_func_draw_to_texture)(void*);
    void* args;
    int window_width;
    int window_height;
    int mode;
} redraw_event_data;

typedef struct {
    const char* file_name;
    int height;
    int width;
    double center_coord_real;
    double center_coord_im;
    double radius;
    int num_iters;
    double rotate_degrees;
    double brightness;
    color_mapping_func red_mapping_func;
    color_mapping_func green_mapping_func;
    color_mapping_func blue_mapping_func;
} mandelbrot_singlecore_args;

typedef struct {
    void* mandelbrot_args;
    
    union {
        mandelbrot_singlecore_func singlecore;
    } p_func_mandelbrot;
    
    int mode;
} mandelbrot_data;

#endif