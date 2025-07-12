#include "color_functions.h"
#include "sdl_manager.h"
#include "palette_manager.h"
#include "window_drawer.h"

int main() {
    int window_width  = 640,
        window_height = 480;
    SDL_Window* window = window_init(window_width, window_height);
    SDL_Renderer* renderer = create_renderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, window_width, window_height);
    SDL_Event event;

    int running = 1;
    int needs_redraw = 1;

    color_palette palette;
    generate_color_palette(&palette, NULL, 1, sin_crescator, log_pe_sin, x_patrat_0_5);

    /*
        1. load default mandelbrot dll
        2. set up interface (buttons and stuff)
        3. draw mandelbrot
    */

    
    // TODO: actually should start with drawing mandelbrot, not palette
    redraw_event_data redraw_info = {renderer, texture, draw_palette_to_texture, (void*)&palette, window_width, window_height};
    SDL_TimerID timer_id = SDL_AddTimer(100, scheduled_redraw, &redraw_info);

    // render_palette_window(NULL, 1, sin_crescator, log_pe_sin, x_patrat_0_5);

    while(running) {
        needs_redraw |= handle_sdl_events(&event, window, &redraw_info, &running, PALETTE);
        if(needs_redraw) {
            // some int for option modes: mandelbrot, palette
            /*
                then a switch based on that int
                depending on case, the redraw_info->args and redraw_info->draw_to_texture are chosen
            */
            update_and_render_texture(&redraw_info);
            needs_redraw = 0;
        }
        SDL_Delay(10);
    }

    SDL_RemoveTimer(timer_id);
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
