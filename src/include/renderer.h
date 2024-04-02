
#ifndef _H_RENDERER
#define _H_RENDERER

#include "raylib.h"
#include "util.h"
#include "game_data.h"
#include "texture_mgr.h"
#include <stddef.h>
#include <stdint.h>

Result renderer_init(ft_data_t* gd, const char* title);
Result renderer_draw_frame(ft_data_t* gd);
Result renderer_destroy(ft_data_t* gd);

#endif
