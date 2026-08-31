#include "animation.hpp"
#include "common.hpp"

void Animation::play()
{
    counter = 0;
    done = false;
}

void Animation::update()
{
    if (done) {
        return;
    }
    const int frame = get_frame();
    if (!loop && frame == length - 1) {
        done = true;
        return;
    }
    counter++;
}

void Animation::draw(float x, float y, float alpha, bool flip_h, bool flip_v) const
{
    const int frame = get_frame();
    const int atlas_index = frame + atlas_index_first;
    draw_sprite(atlas_index, x, y, alpha, flip_h, flip_v);
}

void Animation::draw_tiled(Grid_Point point, float alpha, bool flip_h, bool flip_v) const
{
    draw(point.col * TILE_W, point.row * TILE_H, alpha, flip_h, flip_v);
}