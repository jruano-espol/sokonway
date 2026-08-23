#pragma once

#include "level.hpp"
#include "player.hpp"
#include "animation.hpp"

struct Game_State {
    RenderTexture2D renderTexture;
    Texture2D textureAtlas;
    Level levels[10];
    Animation animation_player;
    Animation animation_portal;
    Animation animation_door;
    int current_level;
    Player player;
};
extern Game_State game;