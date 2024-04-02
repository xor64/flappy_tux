
#ifndef _H_TEXTUREMGR
#define _H_TEXTUREMGR

#include "raylib.h"
#include "util.h"

typedef struct {
    Texture2D tux;
} ft_textures_t;

Result tx_mgr_load_textures(ft_textures_t* textures);
Result tx_mgr_free_textures(ft_textures_t* textures);



#endif