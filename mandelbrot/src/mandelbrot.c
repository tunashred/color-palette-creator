#include "common.h"
#include "mandelbrot.h"

void draw_mandelbrot_to_texture(void* args) {
    redraw_event_data* data = (redraw_event_data*) args;
    int window_height = data->window_height, window_width = data->window_width;

    mandelbrot_data* m_args = (mandelbrot_data*) data->args;
    uint32_t* picture;
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

    void* pixels;
    int pitch;

    SDL_LockTexture(data->texture, NULL, &pixels, &pitch);
    uint32_t* pixel_data = (uint32_t*) pixels;

    SDL_PixelFormat* format = SDL_AllocFormat(SDL_PIXELFORMAT_RGBA8888);

    int picture_index;
    for (int i = 0; i < window_height; ++i) {
        for (int j = 0; j < window_width; ++j) {
            picture_index = j + (i * j);
            uint8_t red   = picture[picture_index++];
            uint8_t green = picture[picture_index++];
            uint8_t blue  = picture[picture_index];
            pixel_data[i * pitch + j] = SDL_MapRGBA(format, red, green, blue, 255);
        }
    }

    SDL_UnlockTexture(data->texture);
    SDL_FreeFormat(format);
    free(picture);
}

void initialize_mandelbrot_data() {

}
