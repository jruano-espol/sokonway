#pragma once

#include <raylib.h>
#include <cstdint>

enum class Tile_Kind : uint8_t {
    Air,
    Player,
    Cell,
    Wall,
    Button,
    Portal,
    Door,
};

int tile_index_from(Tile_Kind kind);

struct Grid_Point {
    int row;
    int col;

    bool in_atlas_bounds();
    static Grid_Point from(int atlas_index);
};

void draw_sprite(int atlas_index, float x, float y);
void draw_tile(Tile_Kind kind, int row, int col);

