#include "player.hpp"
#include "common.hpp"
#include "game_state.hpp"
#include "level.hpp"
#include "raylib.h"

#define is_left_pressed() (IsKeyPressedOrRepeat(KEY_A) || IsKeyPressedOrRepeat(KEY_LEFT))
#define is_right_pressed() (IsKeyPressedOrRepeat(KEY_D) || IsKeyPressedOrRepeat(KEY_RIGHT))
#define is_up_pressed() (IsKeyPressedOrRepeat(KEY_W) || IsKeyPressedOrRepeat(KEY_UP))
#define is_down_pressed() (IsKeyPressedOrRepeat(KEY_S) || IsKeyPressedOrRepeat(KEY_DOWN))

void Player::update()
{
    Level &level = game.levels[game.current_level];

    if (IsKeyPressed(KEY_R)) {
        level.reload();
        grid_position = level.initial_player_position;
        return;
    }
    if (IsKeyPressed(KEY_F)) {
        level.toggle_flag(Level_Flag::Simulation_Speed_Fast);
    }
    if (IsKeyPressed(KEY_SPACE)) {
        int index = level.lever_positions.linear_search_index_of(grid_position);
        if (index >= 0) {
            level.toggle_flag(Level_Flag::Running_Conways_Game_Of_Life);
        }
    }

    Grid_Point direction = {0, 0};
    if (is_left_pressed()) {
        direction.col -= 1;
        facing_left = true;
    }
    if (is_right_pressed()) {
        direction.col += 1;
        facing_left = false;
    }
    if (is_up_pressed()) {
        direction.row -= 1;
    }
    if (is_down_pressed()) {
        direction.row += 1;
    }

    if (direction != Grid_Point{0, 0}) {
        const Grid_Point new_position = grid_position + direction;
        const Tile_Kind tile_there = level.at(new_position);
        bool can_move_there = level.in_bounds(new_position);

        switch (tile_there) {
        case Tile_Kind::Portal: {
            bool has_next_level =
                game.current_level + 1 < LEVEL_COUNT &&
                game.levels[game.current_level + 1].has_flag(Level_Flag::Is_Loaded);

            if (has_next_level) {
                game.current_level++;
                grid_position = game.levels[game.current_level].initial_player_position;
                return;
            }
        } break;
        case Tile_Kind::Button: {
            Button *p_button = level.find_button(new_position);
            if (p_button && !p_button->pressed) {
                p_button->pressed = true;
                level.remove_door_at(p_button->corresponding_door_point);
            }
        } break;
        case Tile_Kind::Wall:
        case Tile_Kind::Door:
            can_move_there = false;
            break;
        default:
            break;
        }

        if (level.has_alive_cell_at(new_position)) {
            const Grid_Point after_target = new_position + direction;
            if (level.has_alive_cell_at(after_target)) {
                can_move_there = false;
            } else {
                level.set_cell(new_position, false);
                level.set_cell(after_target, true);
                memcpy(level.cells_last_generation, level.cells, level.width * level.height);
            }
        }

        if (can_move_there) {
            grid_position = new_position;
        }
    }
}

void Player::draw()
{
    game.animation_player.draw_tiled(grid_position, 1.0f, facing_left);
}