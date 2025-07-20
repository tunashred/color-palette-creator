#include "sdl_manager.h"
#include "palette_manager.h"

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

SDL_Renderer* create_renderer(SDL_Window* window, int index, uint32_t flags) {
    SDL_Renderer* renderer = SDL_CreateRenderer(window, index, flags);
    if(!renderer) {
        printf("SDL_CreateRenderer error: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        exit(EXIT_FAILURE);
    }
    return renderer;
}

uint32_t scheduled_redraw(uint32_t interval, void* args) {
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

// TODO: refactor this monstrosity
void handle_sdl_events(SDL_Event* event, SDL_Window* window, redraw_event_data* data, int mode) {
    color_palette* palette;

    while(SDL_PollEvent(event)) {
        if(event->type == SDL_QUIT) {
            SDL_LockMutex(data->redraw_mutex);
            data->running = false;
            data->cancel_current_draw = false;

            SDL_CondSignal(data->redraw_cond);
            SDL_UnlockMutex(data->redraw_mutex);
        }
        // more cases for window?
        else if(event->type == SDL_WINDOWEVENT && event->window.event == SDL_WINDOWEVENT_RESIZED) {
            int new_width, new_height;
            SDL_GetWindowSize(window, &new_width, &new_height);

            SDL_LockMutex(data->redraw_mutex);
            data->cancel_current_draw = true;
            while (data->currently_drawing) {
                SDL_CondWait(data->redraw_cond, data->redraw_mutex);
            }
            SDL_UnlockMutex(data->redraw_mutex);

            SDL_Delay(100);
            
            SDL_LockMutex(data->redraw_mutex);

            data->window_width = new_width;
            data->window_height = new_height;

            if (data->texture) {
                SDL_DestroyTexture(data->texture);
                data->texture = NULL;
            }
            data->texture = SDL_CreateTexture(data->renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, new_width, new_height);
            
            data->cancel_current_draw = false;
            data->render_ready = false;

            SDL_CondSignal(data->redraw_cond);
            SDL_UnlockMutex(data->redraw_mutex);
        }
        
        // possible optimization trick: if palette is changed, maybe just update the colors(pixels) of the picture instead of redrawing it
        switch (mode) {
            case MANDELBROT_SINGLECORE:
                // mandelbrot specific actions: mousewheel, clicking buttons etc.
                break;
            case PALETTE:
                palette = (color_palette*) data->args;
                if(event->type == SDL_USEREVENT) { // timed redraw event
                    SDL_LockMutex(data->redraw_mutex);
                    SDL_CondSignal(data->redraw_cond);
                    SDL_UnlockMutex(data->redraw_mutex);
                }
                else if(event->type == SDL_KEYDOWN) { // changing colors with R, G, B key presses
                    if(event->key.keysym.sym == SDLK_x) {
                        export_palette(palette);
                    } else {
                        color_function_changer(event->key.keysym.sym, palette);
                    }
                }
                break;
            default:
                break;
        }
    }
}
