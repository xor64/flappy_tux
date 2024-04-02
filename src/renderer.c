#include "renderer.h"

Result renderer_init(ft_data_t* gd, const char* title) {
    InitWindow(gd->w_width, gd->w_height, title);
    SetTargetFPS(60);

    tx_mgr_load_textures(&gd->textures);

    gd->tux.pos.x = (gd->w_width * 0.2) - ((gd->textures.tux.width * 0.1) / 2);
    gd->tux.pos.y = (gd->w_height * 0.5) - ((gd->textures.tux.height * 0.1) / 2);

    return Ok;
}

Result renderer_draw_frame(ft_data_t* gd) {
    BeginDrawing();
    {
        ClearBackground(GRAY);

        DrawTextureEx(gd->textures.tux, gd->tux.pos, gd->tux.rot, 0.1f, WHITE);

        DrawText("Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY);
    }
    EndDrawing();
    return Ok;
}

Result renderer_destroy(ft_data_t* gd) {
    tx_mgr_free_textures(&gd->textures);
    CloseWindow();
    return Ok;
}