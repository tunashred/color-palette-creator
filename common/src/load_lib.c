#include "load_lib.h"
#include "common.h"
#include <dlfcn.h>

#define LOAD_DLL(path) dlopen(path, RTLD_LAZY)
#define UNLOAD_DLL(handle) dlclose(handle)

    mandelbrot_singlecore_func get_mandelbrot_singlecore_func(void* handle) {
        return (mandelbrot_singlecore_func) dlsym(handle, "mandelbrot_around_center");
    }
