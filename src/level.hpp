#pragma once

#include "atlas.hpp"
#include <cassert>

struct Level {
    int width = 0;
    int height = 0;
    uint8_t *data = nullptr;
    uint8_t *initial_data = nullptr;
    Grid_Point initial_player_position = {};

    ~Level();

    bool in_bounds(int row, int col) const
    {
        return (0 <= col && col < width) && (0 <= row && row < height);
    }

    bool in_bounds(Grid_Point point) const
    {
        return (0 <= point.col && point.col < width) && (0 <= point.row && point.row < height);
    }

    Tile_Kind at(int row, int col);
    void set(int row, int col, Tile_Kind kind);

    void load(const char *identifier);
    void draw();
};
