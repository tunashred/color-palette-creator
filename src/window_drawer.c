#include "window_drawer.h"

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

void update_and_render_texture(SDL_Renderer* renderer, SDL_Texture* texture, color_palette* palette, int window_width, int window_height) {
    draw_palette_to_texture(texture, window_width, window_height, palette->r, palette->g, palette->b, palette->rgb);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
}
