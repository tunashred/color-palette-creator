#include "mandelbrot/mandelbrot.h"
#include "common/color_functions.h"
#include "common/common.h"

// It no longer uses the texture. What if the mandelbrot functions could somehow write the textures too?
// Could be faster than having just the main thread doing the pixel work
void draw_mandelbrot_to_texture(void* args) {
    redraw_event_data* data = (redraw_event_data*) args;
    int window_height = data->window_height, window_width = data->window_width;

    mandelbrot_data* m_args = (mandelbrot_data*) data->args;

    if (m_args->picture) {
        free(m_args->picture);
        m_args->picture = NULL;
    }
    
    uint32_t* picture = NULL;
    if (data->mode == MANDELBROT_SINGLECORE) {
        mandelbrot_singlecore_args* mandelbrot_args = (mandelbrot_singlecore_args*) m_args->mandelbrot_args;
        picture =
            m_args->p_func_mandelbrot.singlecore(
                NULL, window_height, window_width, mandelbrot_args->center_coord_real, mandelbrot_args->center_coord_im,
                mandelbrot_args->radius, mandelbrot_args->num_iters, mandelbrot_args->rotate_degrees,
                mandelbrot_args->brightness, mandelbrot_args->red_mapping_func, mandelbrot_args->green_mapping_func,
                mandelbrot_args->blue_mapping_func
            );
    }

    if (!picture) {
        fprintf(stderr, "Unable to generate picture\n");
        return;
    }

    m_args->picture = picture;
}

mandelbrot_data* initialize_mandelbrot_singlecore_data(int window_height, int window_width) {
    mandelbrot_singlecore_args* singlecore_args = (mandelbrot_singlecore_args*) malloc(sizeof *singlecore_args);
    *singlecore_args = (mandelbrot_singlecore_args) {.file_name          = NULL,
                                                     .height             = window_height,
                                                     .width              = window_width,
                                                     .center_coord_real  = -0.43,
                                                     .center_coord_im    = -0.1,
                                                     .radius             = 1,
                                                     .num_iters          = 1500,
                                                     .rotate_degrees     = 0,
                                                     .brightness         = 0.3,
                                                     .red_mapping_func   = sin_x_la_4,
                                                     .green_mapping_func = unu_minus_unu_pe_x,
                                                     .blue_mapping_func  = x_patrat_0_1_to_0_2};
    
    mandelbrot_data* default_single_core_data = (mandelbrot_data*) malloc(sizeof *default_single_core_data);
    *default_single_core_data = (mandelbrot_data) {.picture           = NULL,
                                                   .mandelbrot_args   = (void*) singlecore_args,
                                                   .p_func_mandelbrot = NULL,
                                                   .mode              = MANDELBROT_SINGLECORE};

    return default_single_core_data;
}
