#include "window_drawer.h"

void update_and_render_texture(redraw_event_data* data) {
    data->p_func_draw_to_texture(data->buffer_data);
    SDL_RenderCopy(data->renderer, data->texture, NULL, NULL);
    SDL_RenderPresent(data->renderer);
}
