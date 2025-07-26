#include <SDL2/SDL_thread.h>
#include <dlfcn.h>

#include "common/color_functions.h"
#include "common/common.h"
#include "common/load_lib.h"
#include "mandelbrot/mandelbrot.h"
#include "window/sdl_manager.h"
#include "palette_creator/palette_manager.h"
#include "window/window_drawer.h"

// TODO: implement a safe malloc function
int main() {
    int window_width  = 640,
        window_height = 480;

    SDL_Window* window = window_init(window_width, window_height);
    SDL_Renderer* renderer = create_renderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    SDL_Texture* texture = create_texture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, window_width, window_height);

    default_window_present(window, renderer);

    color_palette palette;
    generate_color_palette(&palette, NULL, 1, sin_crescator, log_pe_sin, x_patrat_0_5);

    // this will be moved to a separate function and handled by events function
    void* handle = dlopen("lib/liblib_mandelbrot_singlecore.so", RTLD_NOW);
    if (!handle) {
        fprintf(stderr, "Error: %s\n", dlerror());
        return 1;
    }

    mandelbrot_data* default_single_core_data = initialize_mandelbrot_singlecore_data(window_height, window_width);
    default_single_core_data->p_func_mandelbrot.singlecore = get_mandelbrot_singlecore_func(handle);

    // TODO: actually should start with drawing mandelbrot, not palette
    // redraw_event_data redraw_info = {renderer, texture, draw_palette_to_texture, (void*)&palette, window_width, window_height, PALETTE};
    SDL_mutex* redraw_mutex = SDL_CreateMutex();
    SDL_cond* redraw_cond = SDL_CreateCond();
    redraw_event_data redraw_info = {renderer, texture, draw_mandelbrot_to_texture, (void*) default_single_core_data,
                                     redraw_mutex, redraw_cond, window_height, window_width, MANDELBROT_SINGLECORE,
                                     true, false, false, false};
    
    SDL_Thread* draw_thread = SDL_CreateThread(draw_worker, "DrawThread", (void*) &redraw_info);
    SDL_DetachThread(draw_thread);

    // trigger initial draw
    SDL_LockMutex(redraw_mutex);
    SDL_CondSignal(redraw_cond);
    SDL_UnlockMutex(redraw_mutex);

    SDL_Event event;
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

    // TODO  : wrap these in functions for each lib
    // TODO 2: need to free handle too?
    SDL_DestroyCond(redraw_cond);
    SDL_DestroyMutex(redraw_mutex);
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
