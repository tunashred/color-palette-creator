#ifndef MANDELBROT_MANDELBROT_H
#define MANDELBROT_MANDELBROT_H

#include "common.h"
#include "color_functions.h"

void draw_mandelbrot_to_texture(void* args);

// mandelbrot_data default_single_core_data;

void initialize_mandelbrot_data();

#endif