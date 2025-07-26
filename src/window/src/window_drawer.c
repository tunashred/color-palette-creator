#include <SDL2/SDL.h>
#include <SDL2/SDL_thread.h>

#include "window/window_drawer.h"
#include "common/color_functions.h"
#include "mandelbrot/mandelbrot.h"
#include "common/common.h"

void default_window_present(SDL_Window* window, SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
    SDL_ShowWindow(window);
}

void draw(redraw_event_data* data) {
    data->p_func_draw_to_texture(data);
}

// TODO: revisit this. Should this be reentrant?
int draw_worker(void* args) {
    redraw_event_data* data = (redraw_event_data*) args;
    mandelbrot_data* mandelbrot = (mandelbrot_data*) data->args;
    SDL_mutex* mutex = data->redraw_mutex;

    while (data->running) {
        SDL_LockMutex(mutex);
        SDL_CondWait(data->redraw_cond, mutex);

        if (!data->running) {
            SDL_UnlockMutex(mutex);
            break;
        }
        data->cancel_current_draw = false;
        data->currently_drawing = true;
        SDL_UnlockMutex(mutex);

        draw(data);

        SDL_LockMutex(mutex);
        data->currently_drawing = false;
        if (!data->cancel_current_draw && data->running) {
            data->render_ready = true;
        }
        SDL_CondSignal(data->redraw_cond);
        SDL_UnlockMutex(mutex);
    }
    return 0;
}

void render_mandelbrot(redraw_event_data* args) {
    redraw_event_data* data = (redraw_event_data*) args;
    mandelbrot_data* mandelbrot = (mandelbrot_data*) data->args;
    uint32_t* picture = mandelbrot->picture;

    if (!picture) {
        fprintf(stderr, "Picture to render is null\n");
        return;
    }

    int window_height = data->window_height, window_width = data->window_width;
    if (!data->texture) {
        fprintf(stderr, "Texture is NULL\n");
        return;
    }
    if (SDL_QueryTexture(data->texture, NULL, NULL, &window_width, &window_height) < 0) {
        fprintf(stderr, "Texture query failed: %s\n", SDL_GetError());
        return;
    }

    void* pixels;
    int pitch;
    SDL_LockTexture(data->texture, NULL, &pixels, &pitch);
    uint32_t* pixel_data = (uint32_t*) pixels;

    SDL_PixelFormat* format = SDL_AllocFormat(SDL_PIXELFORMAT_RGBA8888);

    // pitch is in bytes but we need pixels
    int texture_width = pitch / 4;
    long array_size = window_width * window_height * RGB_CHANNELS;
    int picture_index;
    for (int i = 0; i < window_height; ++i) {
        for (int j = 0; j < window_width; ++j) {
            picture_index = (j + i * window_width) * RGB_CHANNELS;
            uint8_t red   = picture[picture_index++];
            uint8_t green = picture[picture_index++];
            uint8_t blue  = picture[picture_index];
            pixel_data[i * texture_width + j] = SDL_MapRGBA(format, red, green, blue, 255);
        }
    }
    SDL_UnlockTexture(data->texture);
    SDL_FreeFormat(format);
}
