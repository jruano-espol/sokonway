#pragma once

#include "atlas.h"
#include <cassert>

struct Level_Grid {
    int width = 0;
    int height = 0;
    uint8_t *data = nullptr;

    ~Level_Grid()
    {
        if (data) {
            delete[] data;
            data = nullptr;
        }
    }

    bool in_bounds(int row, int col)
    {
        return (0 <= col && col < width) && (0 <= row && row < height);
    }

    Tile_Kind at(int row, int col)
    {
        assert(in_bounds(row, col));
        uint8_t value = data[row * width + col];
        return Tile_Kind{value};
    }

    void set(int row, int col, Tile_Kind kind)
    {
        assert(in_bounds(row, col));
        uint8_t value = (uint8_t)kind;
        data[row * width + col] = value;
    }

    void load(const char *identifier);
    void draw();
};
