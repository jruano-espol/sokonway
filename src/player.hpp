#pragma once

#include "atlas.hpp"
#include "animation.hpp"

struct Player {
    Animation animation = Animation::from(Tile_Kind::Player, true, 20);
    Grid_Point grid_position = {0, 0};

    void update();
    void draw();
};