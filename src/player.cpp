#include "player.hpp"
#include "common.hpp"
#include "game_state.hpp"

#define is_left_pressed() (IsKeyPressedOrRepeat(KEY_A) || IsKeyPressedOrRepeat(KEY_LEFT))
#define is_right_pressed() (IsKeyPressedOrRepeat(KEY_D) || IsKeyPressedOrRepeat(KEY_RIGHT))
#define is_up_pressed() (IsKeyPressedOrRepeat(KEY_W) || IsKeyPressedOrRepeat(KEY_UP))
#define is_down_pressed() (IsKeyPressedOrRepeat(KEY_S) || IsKeyPressedOrRepeat(KEY_DOWN))

void Player::update()
{
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

    Level &level = game.levels[game.current_level];

    if (direction != Grid_Point{0, 0}) {
        const Grid_Point new_position = grid_position + direction;
        const Tile_Kind tile_there = level.at(new_position);

        bool can_move_there = level.in_bounds(new_position);
        can_move_there &= tile_there != Tile_Kind::Wall;
        can_move_there &= tile_there != Tile_Kind::Door;

        if (level.at(new_position) == Tile_Kind::Button) {
            Button *p_button = level.find_button(new_position);
            if (p_button && !p_button->pressed) {
                p_button->pressed = true;
                level.remove_door_at(p_button->corresponding_door_point);
            }
        }

        if (can_move_there) {
            grid_position = new_position;
        }
    }
}

void Player::draw()
{
    game.animation_player.draw_tiled(grid_position);
}