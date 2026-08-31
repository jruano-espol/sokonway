#pragma once

#include "level.hpp"
#include "player.hpp"
#include "animation.hpp"

#define LEVEL_COUNT 10

struct Game_State {
    RenderTexture2D renderTexture;
    Texture2D textureAtlas;
    Level levels[LEVEL_COUNT];
    Animation animation_player;
    Animation animation_portal;
    Animation animation_door;
    int current_level;
    Player player;
};
extern Game_State game;