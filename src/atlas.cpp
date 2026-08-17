#include "atlas.h"
#include "common.h"
#include <cassert>

int tile_index_from(Tile_Kind kind)
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

bool Grid_Point::in_atlas_bounds()
{
    int rows = game.textureAtlas.height / TILE_H;
    int cols = game.textureAtlas.width  / TILE_W;
    return (0 <= row && row < rows) && (0 <= col && col < cols);
}

Grid_Point Grid_Point::from(int atlas_index)
{
    int rows = game.textureAtlas.height / TILE_H;
    int cols = game.textureAtlas.width  / TILE_W;
    Grid_Point result = {
        .row = atlas_index / cols,
        .col = atlas_index % cols,
    };
    assert(result.in_atlas_bounds());
    return result;
}

void draw_sprite(int atlas_index, float x, float y)
{
    if (atlas_index >= 0) {
        Grid_Point point = Grid_Point::from(atlas_index);
        Rectangle source = {(float)(point.col * TILE_W), (float)(point.row * TILE_H), TILE_W, TILE_H};
        Rectangle dest = {x, y, TILE_W, TILE_H};
        DrawTexturePro(game.textureAtlas, source, dest, {0, 0}, 0, WHITE);
    }
}

void draw_tile(Tile_Kind kind, int row, int col)
{
    float x = col * TILE_W;
    float y = row * TILE_H;
    draw_sprite(tile_index_from(kind), x, y);
}
