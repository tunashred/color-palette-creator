#ifndef LOAD_LIB_H
#define LOAD_LIB_H

#include "common.h"
#include "mandelbrot.h"

mandelbrot_singlecore_func get_mandelbrot_singlecore_func(void* handle);

#endif