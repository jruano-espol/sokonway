#pragma once

#include "atlas.hpp"

struct Animation {
    int atlas_index_first; // The index in the atlas of the first frame of the animation.
    int length;   // The total amount of frames of the animation.
    int duration; // The amount of in-game frames that it takes to complete one animation frame.
    int counter;  // Used to determine the current frame of the animation.
    bool loop;    // If the animation should loop or not.
    bool done;    // If the animation is done.

    static constexpr Animation from(Tile_Kind kind, bool loop, int duration)
    {
        return {
            .atlas_index_first = atlas_index_from(kind),
            .length = animation_frame_count_of(kind),
            .duration = duration,
            .loop = loop,
        };
    }

    int get_frame() const { return (counter / duration) % length; }

    void play();
    void update();
    void draw(int x, int y) const;
    void draw_tiled(Grid_Point point) const;
};