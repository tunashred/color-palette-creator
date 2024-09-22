#include "window_drawer.h"

SDL_Window* window_init() {
    if(SDL_InitSubSystem(SDL_INIT_VIDEO | SDL_INIT_TIMER)) {
        printf("SDL_InitSubSystem error: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }
    
    SDL_Window* window = SDL_CreateWindow(
                                            "Color palette creator", SDL_WINDOWPOS_CENTERED,
                                            SDL_WINDOWPOS_CENTERED, 640, 480,
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
