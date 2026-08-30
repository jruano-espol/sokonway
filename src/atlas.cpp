#include "atlas.hpp"
#include "common.hpp"
#include "game_state.hpp"
#include "raylib.h"
#include <cassert>

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

void draw_sprite(int atlas_index, float x, float y, float alpha, bool flip_h, bool flip_v)
{
    if (atlas_index >= 0) {
        Grid_Point point = Grid_Point::from(atlas_index);
        float sign_w = flip_h ? -1.0f : 1.0f;
        float sign_h = flip_v ? -1.0f : 1.0f;
        Rectangle source = {
            (float)(point.col * TILE_W),
            (float)(point.row * TILE_H),
            TILE_W * sign_w,
            TILE_H * sign_h,
        };
        Rectangle dest = {x, y, TILE_W, TILE_H};
        Color tint = ColorAlpha(WHITE, alpha);
        DrawTexturePro(game.textureAtlas, source, dest, {0, 0}, 0, tint);
    }
}

void draw_sprite(int atlas_index, Grid_Point point, float alpha, bool flip_h, bool flip_v)
{
    float x = point.col * TILE_W;
    float y = point.row * TILE_H;
    draw_sprite(atlas_index, x, y, alpha, flip_h, flip_v);
}

void draw_tile(Tile_Kind kind, int row, int col, float alpha, bool hide_interactables)
{
    if (hide_interactables) {
        switch (kind) {
        case Tile_Kind::Door:
        case Tile_Kind::Portal:
        case Tile_Kind::Button:
            return;
        default:
            break;
        }
    }
    float x = col * TILE_W;
    float y = row * TILE_H;
    draw_sprite(atlas_index_from(kind), x, y, alpha);
}
