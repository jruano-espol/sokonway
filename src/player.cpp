#include "player.hpp"
#include "common.hpp"

#define is_left_pressed() (IsKeyPressedOrRepeat(KEY_A) || IsKeyPressedOrRepeat(KEY_LEFT))
#define is_right_pressed() (IsKeyPressedOrRepeat(KEY_D) || IsKeyPressedOrRepeat(KEY_RIGHT))
#define is_up_pressed() (IsKeyPressedOrRepeat(KEY_W) || IsKeyPressedOrRepeat(KEY_UP))
#define is_down_pressed() (IsKeyPressedOrRepeat(KEY_S) || IsKeyPressedOrRepeat(KEY_DOWN))

void Player::update()
{
    animation.update();

    Grid_Point direction = {0, 0};
    if (is_left_pressed()) {
        direction = {0, -1};
    }
    if (is_right_pressed()) {
        direction = {0, 1};
    }
    if (is_up_pressed()) {
        direction = {-1, 0};
    }
    if (is_down_pressed()) {
        direction = {1, 0};
    }

    const Level &level = game.levels[game.current_level];

    if (direction != Grid_Point{0, 0}) {
        Grid_Point new_position = grid_position + direction;
        if (level.in_bounds(new_position)) {
            grid_position = new_position;
        }
    }
}

void Player::draw()
{
    animation.draw(grid_position.col * TILE_W, grid_position.row * TILE_H );
}