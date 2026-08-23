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

constexpr int atlas_index_from(Tile_Kind kind)
{
    switch (kind) {
    case Tile_Kind::Air: return -1;
    case Tile_Kind::Player: return 0;
    case Tile_Kind::Cell: return 2;
    case Tile_Kind::Wall: return 3;
    case Tile_Kind::Button: return 6;
    case Tile_Kind::Portal: return 8;
    case Tile_Kind::Door: return 12;
    }
    return -1;
}

constexpr int animation_frame_count_of(Tile_Kind kind)
{
    switch (kind) {
    case Tile_Kind::Player: return 2;
    case Tile_Kind::Portal: return 4;
    case Tile_Kind::Door: return 6;
    default: return 0;
    }
    return 0;
}

struct Grid_Point {
    int row;
    int col;

    bool in_atlas_bounds();
    static Grid_Point from(int atlas_index);

    bool operator==(Grid_Point other) const
    {
        return this->row == other.row && this->col == other.col;
    }

    Grid_Point operator+(Grid_Point other) const
    {
        return Grid_Point{this->row + other.row, this->col + other.col};
    }
};

void draw_sprite(int atlas_index, float x, float y, float alpha = 1.0f);
void draw_sprite(int atlas_index, Grid_Point point, float alpha = 1.0f);
void draw_tile(Tile_Kind kind, int row, int col, float alpha = 1.0f, bool hide_interactables = false);

