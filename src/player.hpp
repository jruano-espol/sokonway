#pragma once

#include "atlas.hpp"

struct Player {
    Grid_Point grid_position = {0, 0};
    bool facing_left = false;

    void update();
    void draw();
};