#include "window_drawer.h"
#include "color_mapping.h"
#include <SDL_timer.h>

int main() {
    create_palette("../mandelbrot/palettes/palette_packs/putine_culori_dar_final_fericit.txt", 1, sin_crescator, log_pe_sin, x_patrat_0_5);

    return 0;
}
