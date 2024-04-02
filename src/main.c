#include <stdio.h>
#include <stdlib.h>

#include "renderer.h"

int main(int argc, char** argv) {
    printf("Hewo world :3\n");

    ft_data_t gd = {0};
    
    gd.w_width = 800;
    gd.w_height = 450;

    renderer_init(&gd, "Flappy tux :3");

    while (!WindowShouldClose()) {
        renderer_draw_frame(&gd);
    }

    renderer_destroy(&gd);
    return 0;
}
