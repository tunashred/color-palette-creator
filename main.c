#include "color_functions.h"
#include "common.h"
#include "load_lib.h"
#include "mandelbrot.h"
#include "sdl_manager.h"
#include "palette_manager.h"
#include "window_drawer.h"
#include <dlfcn.h>

int main() {
    int window_width  = 640,
        window_height = 480;
    SDL_Window* window = window_init(window_width, window_height);
    SDL_Renderer* renderer = create_renderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, window_width, window_height);
    SDL_Event event;
    
    if (!texture) {
        fprintf(stderr, "Failed to create texture: %s\n", SDL_GetError());
        return 1;
    }

    int running = 1;
    int needs_redraw = 1;

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

    mandelbrot_data default_single_core_data = {(void*) &singlecore_args,
                                                    .p_func_mandelbrot = NULL,
                                                    .mode = MANDELBROT_SINGLECORE};

    default_single_core_data.p_func_mandelbrot.singlecore = get_mandelbrot_singlecore_func(handle);

    // TODO: actually should start with drawing mandelbrot, not palette
    // redraw_event_data redraw_info = {renderer, texture, draw_palette_to_texture, (void*)&palette, window_width, window_height, PALETTE};
    redraw_event_data redraw_info = {renderer, texture, draw_mandelbrot_to_texture, (void*) &default_single_core_data, window_width, window_height, MANDELBROT_SINGLECORE};
    SDL_TimerID timer_id = SDL_AddTimer(2500, scheduled_redraw, &redraw_info);

    // render_palette_window(NULL, 1, sin_crescator, log_pe_sin, x_patrat_0_5);

    while(running) {
        needs_redraw |= handle_sdl_events(&event, window, &redraw_info, &running, MANDELBROT_SINGLECORE);
        if(needs_redraw) {
            // some int for option modes: mandelbrot, palette
            /*
                then a switch based on that int
                depending on case, the redraw_info->args and redraw_info->draw_to_texture are chosen
            */
            update_and_render_texture(&redraw_info);
            needs_redraw = 0;
        }
        SDL_Delay(2500);
    }

    SDL_RemoveTimer(timer_id);
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
