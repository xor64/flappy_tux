
#include "texture_mgr.h"


Result tx_mgr_load_textures(ft_textures_t* textures) {
    Image img = {0};

    img = LoadImage("assets/tux.png");
    textures->tux = LoadTextureFromImage(img);
    UnloadImage(img);

    return Ok;
}

Result tx_mgr_free_textures(ft_textures_t* textures) {
    UnloadTexture(textures->tux);
}

