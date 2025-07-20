#include <SDL2/SDL_thread.h>
#include <dlfcn.h>

#include "color_functions.h"
#include "common.h"
#include "load_lib.h"
#include "mandelbrot.h"
#include "sdl_manager.h"
#include "palette_manager.h"
#include "window_drawer.h"

// TODO: globally add another level of directories for include dirs, dirs which are named after the corresponding lib
int main() {
    int window_width  = 640,
        window_height = 480;
    SDL_Window* window = window_init(window_width, window_height);
    SDL_Renderer* renderer = create_renderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, window_width, window_height);
    SDL_Event event;
    
    // TODO: make wrappers for stuff like this
    if (!texture) {
        fprintf(stderr, "Failed to create texture: %s\n", SDL_GetError());
        return 1;
    }

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
    SDL_ShowWindow(window);

    color_palette palette;
    generate_color_palette(&palette, NULL, 1, sin_crescator, log_pe_sin, x_patrat_0_5);

    /*
        1. load default mandelbrot dll
        2. set up interface (buttons and stuff)
        3. draw mandelbrot
    */
    void* handle = dlopen("lib/liblib_mandelbrot_singlecore.so", RTLD_NOW);
    if (!handle) {
        fprintf(stderr, "Error: %s\n", dlerror());
        return 1;
    }

    initialize_mandelbrot_data();

    mandelbrot_singlecore_args singlecore_args = {
                                                    NULL, window_height, window_width, -0.43,
                                                    -0.1, 1, 1500, 0, 0.3,
                                                    sin_x_la_4, unu_minus_unu_pe_x, x_patrat_0_1_to_0_2
                                                    };

    mandelbrot_data default_single_core_data = {NULL,
                                                (void*) &singlecore_args,
                                                .p_func_mandelbrot = NULL,
                                                .mode = MANDELBROT_SINGLECORE};

    default_single_core_data.p_func_mandelbrot.singlecore = get_mandelbrot_singlecore_func(handle);

    // TODO: actually should start with drawing mandelbrot, not palette
    // redraw_event_data redraw_info = {renderer, texture, draw_palette_to_texture, (void*)&palette, window_width, window_height, PALETTE};
    SDL_mutex* redraw_mutex = SDL_CreateMutex();
    SDL_cond* redraw_cond = SDL_CreateCond();
    redraw_event_data redraw_info = {renderer, texture, draw_mandelbrot_to_texture, (void*) &default_single_core_data, redraw_mutex, redraw_cond, window_width, window_height, true, false, false, false, MANDELBROT_SINGLECORE};
    
    SDL_Thread* draw_thread = SDL_CreateThread(draw_worker, "DrawThread", (void*) &redraw_info);
    SDL_DetachThread(draw_thread);

    // trigger initial draw
    SDL_LockMutex(redraw_mutex);
    SDL_CondSignal(redraw_cond);
    SDL_UnlockMutex(redraw_mutex);

    while(redraw_info.running) {
        handle_sdl_events(&event, window, &redraw_info, MANDELBROT_SINGLECORE);

        SDL_Delay(100);

        // TODO: to add check for mandelbrot or color palette
        SDL_LockMutex(redraw_mutex);
        if (redraw_info.render_ready) {
            render_mandelbrot(&redraw_info);
            SDL_RenderCopy(redraw_info.renderer, redraw_info.texture, NULL, NULL);
            SDL_RenderPresent(redraw_info.renderer);
            redraw_info.render_ready = false;
        }
        SDL_UnlockMutex(redraw_mutex);
    }

    // TODO: wrap these in functions for each lib
    SDL_DestroyCond(redraw_cond);
    SDL_DestroyMutex(redraw_mutex);
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
