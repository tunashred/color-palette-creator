#include "window_drawer.h"
#include "color_mapping.h"
#include <time.h>

int main() {
    SDL_Window* window = window_init();
    SDL_Renderer* renderer = create_renderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    SDL_Event event;
    int running = 1;

    color_palette palette;
    generate_color_palette(&palette, NULL, 1, sin_crescator, log_pe_sin, x_patrat_0_5);

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = 0;
            }
            else if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_RESIZED) {
                int new_width, new_height;
                SDL_GetWindowSize(window, &new_width, &new_height);
                
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                SDL_RenderClear(renderer);
                
                draw_palette(renderer, new_width, new_height, palette.r, palette.g, palette.b, palette.rgb);
            }
        }
        SDL_Delay(100);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
