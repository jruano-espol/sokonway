#include "animation.hpp"

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

void Animation::draw(int x, int y)
{
    const int frame = get_frame();
    const int atlas_index = frame + atlas_index_first;
    draw_sprite(atlas_index, x, y);
}