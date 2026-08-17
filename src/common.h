#pragma once

#include <raylib.h>
#include "level.h"

#define TILE_W 8
#define TILE_H 8

#define LEVEL_TILE_COLS 40
#define LEVEL_TILE_ROWS 22

#define GAME_SCALE 2
#define SCREEN_W (TILE_W * LEVEL_TILE_COLS)
#define SCREEN_H (TILE_H * LEVEL_TILE_ROWS)

struct Game_State {
    RenderTexture2D renderTexture;
    Texture2D textureAtlas;
    Level_Grid levels[10];
};
extern Game_State game;
