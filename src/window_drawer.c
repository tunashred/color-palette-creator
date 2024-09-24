#include "window_drawer.h"

SDL_Window* window_init(int window_width, int window_height) {
    if(SDL_InitSubSystem(SDL_INIT_VIDEO | SDL_INIT_TIMER)) {
        printf("SDL_InitSubSystem error: %s\n", SDL_GetError());
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
        SDL_DestroyWindow(window);
        printf("SDL_CreateRenderer error: %s\n", SDL_GetError());
        SDL_Quit();
        exit(EXIT_FAILURE);
    }
    return renderer;
}

void draw_palette_to_texture(SDL_Texture* texture, int window_width,
                            int window_height, uint8_t* red_array, uint8_t* green_array,
                            uint8_t* blue_array, uint8_t (*combined_array)[3]) {
    void* pixels;
    int pitch;

    SDL_LockTexture(texture, NULL, &pixels, &pitch);
    uint32_t* pixel_data = (uint32_t*) pixels;

    int stripe_height = window_height / 4;
    int palette_size = 1500;

    SDL_PixelFormat* format = SDL_AllocFormat(SDL_PIXELFORMAT_RGBA8888);

    // Red stripe
    for (int y = 0; y < stripe_height; ++y) {
        for (int x = 0; x < window_width; ++x) {
            int palette_index = (x * palette_size) / window_width;
            uint8_t red = red_array[palette_index];
            pixel_data[y * (pitch / 4) + x] = SDL_MapRGBA(format, red, 0, 0, 255);
        }
    }

    // Green stripe
    for (int y = stripe_height; y < 2 * stripe_height; ++y) {
        for (int x = 0; x < window_width; ++x) {
            int palette_index = (x * palette_size) / window_width;
            uint8_t green = green_array[palette_index];
            pixel_data[y * (pitch / 4) + x] = SDL_MapRGBA(format, 0, green, 0, 255);
        }
    }

    // Blue stripe
    for (int y = 2 * stripe_height; y < 3 * stripe_height; ++y) {
        for (int x = 0; x < window_width; ++x) {
            int palette_index = (x * palette_size) / window_width;
            uint8_t blue = blue_array[palette_index];
            pixel_data[y * (pitch / 4) + x] = SDL_MapRGBA(format, 0, 0, blue, 255);
        }
    }

    // RGB stripe
    for (int y = 3 * stripe_height; y < 4 * stripe_height; ++y) {
        for (int x = 0; x < window_width; ++x) {
            int palette_index = (x * palette_size) / window_width;
            uint8_t red = combined_array[palette_index][0];
            uint8_t green = combined_array[palette_index][1];
            uint8_t blue = combined_array[palette_index][2];
            pixel_data[y * (pitch / 4) + x] = SDL_MapRGBA(format, red, green, blue, 255);
        }
    }

    SDL_UnlockTexture(texture);
    SDL_FreeFormat(format);
}

Uint32 trigger_redraw_event(Uint32 interval, void* args) {
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

void color_function_changer(int pressed_key, color_palette* palette) {
    switch(pressed_key) {
        case SDLK_r:

            break;

        case SDLK_g:

            break;

        case SDLK_b:

            break;

        default: break;
    }

}

void create_palette(
    char* file_name, double brightness_rate, uint8_t (*red_func)(int, int),
    uint8_t (*green_func)(int, int), uint8_t (*blue_func)(int, int)
) {
    int window_width  = 640,
        window_height = 480;
    
    SDL_Window* window = window_init(window_width, window_height);
    SDL_Renderer* renderer = create_renderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    SDL_Event event;
    int running = 1;
    int needs_redraw = 1;

    color_palette palette;
    uint8_t (*r_func) (int, int) = red_func,
            (*g_func) (int, int) = green_func,
            (*b_func) (int, int) = blue_func;
    generate_color_palette(&palette, file_name, brightness_rate, r_func, g_func, b_func);

    SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, window_width, window_height);

    redraw_event_data redraw_info = {renderer, &palette, window_width, window_height, texture};

    SDL_TimerID timer_id = SDL_AddTimer(100, trigger_redraw_event, &redraw_info);

    while(running) {
        while(SDL_PollEvent(&event)) {
            if(event.type == SDL_QUIT) {
                running = 0;
            }
            else if(event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_RESIZED) {
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
            // else if(event.type == SDL_KEYDOWN) {
            //     color_function_changer(event.key.keysym.sym, &palette);
            // }
        }
        
        if(needs_redraw) {
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
}