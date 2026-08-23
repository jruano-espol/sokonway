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

void Animation::draw(float x, float y, float alpha) const
{
    const int frame = get_frame();
    const int atlas_index = frame + atlas_index_first;
    draw_sprite(atlas_index, x, y, alpha);
}

void Animation::draw_tiled(Grid_Point point, float alpha) const
{
    draw(point.col * TILE_W, point.row * TILE_H, alpha);
}