#include "sdl_manager.h"
#include "window_drawer.h"

SDL_Window* window_init(int window_width, int window_height) {
    if(SDL_InitSubSystem(SDL_INIT_VIDEO | SDL_INIT_TIMER)) {
        printf("SDL_InitSubSystem error: %s\n", SDL_GetError());
        SDL_Quit();
        exit(EXIT_FAILURE);
    }
    
    SDL_Window* window = SDL_CreateWindow(
                                            "Color palette creator", SDL_WINDOWPOS_UNDEFINED,
                                            SDL_WINDOWPOS_UNDEFINED, window_width, window_height,
                                            SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE
                                        );

    if(!window) {
        printf("SDL_CreateWindow error: %s\n", SDL_GetError());
        SDL_Quit();
        exit(EXIT_FAILURE);
    }

    return window;
}

SDL_Renderer* create_renderer(SDL_Window* window, int index, Uint32 flags) {
    SDL_Renderer* renderer = SDL_CreateRenderer(window, index, flags);
    if(!renderer) {
        printf("SDL_CreateRenderer error: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        exit(EXIT_FAILURE);
    }
    return renderer;
}

Uint32 scheduled_redraw(Uint32 interval, void* args) {
    SDL_Event event;
    SDL_UserEvent user_event;

    user_event.type = SDL_USEREVENT;
    user_event.code = 0;
    user_event.data1 = args;
    user_event.data2 = NULL;

    event.type = SDL_USEREVENT;
    event.user = user_event;

    SDL_PushEvent(&event);

    return interval;
}

int handle_sdl_events(SDL_Event* event, int* running, SDL_Window* window, redraw_event_data* redraw_info, color_palette* palette, SDL_Texture** texture) {
    int needs_redraw = 0;

    while(SDL_PollEvent(event)) {
        if(event->type == SDL_QUIT) {
            *running = 0;
        }
        else if(event->type == SDL_WINDOWEVENT && event->window.event == SDL_WINDOWEVENT_RESIZED) {
            int new_width, new_height;
            SDL_GetWindowSize(window, &new_width, &new_height);

            redraw_info->window_width = new_width;
            redraw_info->window_height = new_height;

            SDL_DestroyTexture(*texture);
            *texture = SDL_CreateTexture(redraw_info->renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, new_width, new_height);
            redraw_info->texture = *texture;

            needs_redraw = 1;
        }
        else if(event->type == SDL_USEREVENT) { // timed redraw event
            needs_redraw = 1;
        }
        else if(event->type == SDL_KEYDOWN) { // changing colors with R, G, B key presses
            if(event->key.keysym.sym == SDLK_x) {
                export_palette(palette);
            } else {
                color_function_changer(event->key.keysym.sym, palette);
            }
        }
    }
    return needs_redraw;
}

void render_palette_window(
    char* file_name, double brightness_rate, uint8_t (*red_func)(int, int),
    uint8_t (*green_func)(int, int), uint8_t (*blue_func)(int, int)
) {
    int window_width  = 640,
        window_height = 480;
    SDL_Window* window = window_init(window_width, window_height);
    SDL_Renderer* renderer = create_renderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, window_width, window_height);
    SDL_Event event;

    int running = 1;
    int needs_redraw = 1;

    color_palette palette;
    generate_color_palette(&palette, file_name, brightness_rate, red_func, green_func, blue_func);

    redraw_event_data redraw_info = {renderer, &palette, window_width, window_height, texture};
    SDL_TimerID timer_id = SDL_AddTimer(100, scheduled_redraw, &redraw_info);

    while(running) {
        needs_redraw |= handle_sdl_events(&event, &running, window, &redraw_info, &palette, &texture);
        if(needs_redraw) {
            update_and_render_texture(renderer, texture, &palette, redraw_info.window_width, redraw_info.window_height);
            needs_redraw = 0;
        }
        SDL_Delay(10);
    }

    SDL_RemoveTimer(timer_id);
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
