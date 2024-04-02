
#ifndef _H_GAMEDATA
#define _H_GAMEDATA

#include "texture_mgr.h"
#include <stddef.h>

typedef struct {
    Vector2 pos;
    float rot;
} ft_entity_t;

typedef struct {
    ft_textures_t textures;
    ft_entity_t tux;
    size_t w_width;
    size_t w_height;
} ft_data_t;




#endif