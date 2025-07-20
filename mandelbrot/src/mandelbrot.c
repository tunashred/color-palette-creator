#include "common.h"
#include "mandelbrot.h"

// It no longer uses the texture. What if the mandelbrot functions could somehow write the textures too?
// Could be faster than having just the main thread doing the pixel work
void draw_mandelbrot_to_texture(void* args) {
    redraw_event_data* data = (redraw_event_data*) args;
    int window_height = data->window_height, window_width = data->window_width;

    mandelbrot_data* m_args = (mandelbrot_data*) data->args;

    free(m_args->picture);
    m_args->picture = NULL;
    
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

void initialize_mandelbrot_data() {

}
