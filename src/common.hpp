#pragma once

#include <raylib.h>
#include "level.hpp"
#include "player.hpp"

#define TILE_W 8
#define TILE_H 8

#define LEVEL_TILE_COLS 40
#define LEVEL_TILE_ROWS 22

static_assert(LEVEL_TILE_COLS > 0);
static_assert(LEVEL_TILE_ROWS > 0);

#define GAME_SCALE 2
#define SCREEN_W (TILE_W * LEVEL_TILE_COLS)
#define SCREEN_H (TILE_H * LEVEL_TILE_ROWS)

#define IsKeyPressedOrRepeat(key) (IsKeyPressed(key) || IsKeyPressedRepeat(key))

struct Game_State {
    RenderTexture2D renderTexture;
    Texture2D textureAtlas;
    Level levels[10];
    int current_level;
    Player player;
};
extern Game_State game;