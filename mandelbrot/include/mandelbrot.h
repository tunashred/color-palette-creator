#ifndef MANDELBROT_MANDELBROT_H
#define MANDELBROT_MANDELBROT_H

#include "common.h"

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

typedef uint32_t* (*mandelbrot_singlecore_func)(const char*, int, int, double, double, double, int, double, double,
                                                color_mapping_func, color_mapping_func, color_mapping_func);

typedef struct {
    uint32_t* picture;
    void* mandelbrot_args;
    
    union {
        mandelbrot_singlecore_func singlecore;
    } p_func_mandelbrot;
    
    int mode;
} mandelbrot_data;

void draw_mandelbrot_to_texture(void* args);

mandelbrot_data* initialize_mandelbrot_singlecore_data(int window_height, int window_width);

#endif