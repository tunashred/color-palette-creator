#include "window_drawer.h"
#include "color_mapping.h"
#include <SDL_timer.h>

int main() {
    SDL_Window* window = window_init();
    SDL_Renderer* renderer = create_renderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    SDL_Event event;
    int running = 1;
    int needs_redraw = 1;

    color_palette palette;
    generate_color_palette(&palette, "../mandelbrot/palettes/palette_packs/putine_culori_dar_final_fericit.txt", 1, sin_crescator, log_pe_sin, x_patrat_0_5);

    int window_width, window_height;
    SDL_GetWindowSize(window, &window_width, &window_height);

    SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, window_width, window_height);

    redraw_event_data redraw_info = {renderer, &palette, window_width, window_height, texture};

    SDL_TimerID timer_id = SDL_AddTimer(100, trigger_redraw_event, &redraw_info);

    while(running) {
        while(SDL_PollEvent(&event)) {
            if(event.type == SDL_QUIT) {
                running = 0;
            }
            else if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_RESIZED) {
                int new_width, new_height;
                SDL_GetWindowSize(window, &new_width, &new_height);

                redraw_info.window_width = new_width;
                redraw_info.window_height = new_height;

                SDL_DestroyTexture(texture);
                texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, new_width, new_height);
                redraw_info.texture = texture;

                needs_redraw = 1;
            }
            else if(event.type == SDL_USEREVENT) { // timed redraw event
                needs_redraw = 1;
            }
        }

        if (needs_redraw) {
            draw_palette_to_texture(texture, redraw_info.window_width, redraw_info.window_height, palette.r, palette.g, palette.b, palette.rgb);
            SDL_RenderCopy(renderer, texture, NULL, NULL);
            SDL_RenderPresent(renderer);
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
