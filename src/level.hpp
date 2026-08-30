#pragma once

#include "animation.hpp"
#include "atlas.hpp"
#include "common.hpp"

template <Tile_Kind kind>
struct PropAnimated {
    Grid_Point point;

    constexpr bool operator==(const PropAnimated &other) const
    {
        return this->point == other.point;
    }

    const Animation &get_animation();
    void draw(float alpha = 1.0f);
};

struct Button {
    Grid_Point point;
    Grid_Point corresponding_door_point;
    bool pressed;
};

enum class Level_Flag : uint8_t {
    Is_Loaded = BIT(0),
    Has_Player = BIT(1),
    Has_Portal = BIT(2),
    Has_Lever  = BIT(3),
    Running_Conways_Game_Of_Life = BIT(4),
};

struct Level {
    Fixed_Array<Button, 8> buttons = {};
    Fixed_Array<PropAnimated<Tile_Kind::Door>, 8> doors = {};
    Bump_Allocator_Fixed allocator = {};
    Grid_Point initial_player_position = {};
    Grid_Point lever_position = {};
    PropAnimated<Tile_Kind::Portal> portal = {};

    uint8_t *initial_data = nullptr;
    uint8_t *tiles = nullptr;
    uint8_t *cells = nullptr;
    uint8_t *cells_last_generation = nullptr;
    size_t generation = 0;
    size_t simulation_timer = 0;
    int id = 0;
    int width = 0;
    int height = 0;
    uint8_t flags = 0;

    constexpr bool in_bounds(int row, int col) const
    {
        return (0 <= col && col < width) && (0 <= row && row < height);
    }

    constexpr bool in_bounds(Grid_Point point) const
    {
        return in_bounds(point.row, point.col);
    }

    constexpr bool has_flag(Level_Flag flag) const
    {
        return (flags & (uint8_t)flag) == (uint8_t)flag;
    }

    constexpr void toggle_flag(Level_Flag flag)
    {
        if (has_flag(flag)) {
            flags ^= (uint8_t)flag;
        } else {
            flags |= (uint8_t)flag;
        }
    }

    Tile_Kind at(int row, int col) const;
    Tile_Kind at(Grid_Point point) const;
    void set(Grid_Point point, Tile_Kind kind);
    void set_initial(Grid_Point point, Tile_Kind kind);

    bool has_alive_cell_on_last_generation_at(Grid_Point point) const;
    bool has_alive_cell_at(Grid_Point point) const;
    void set_cell(Grid_Point point, bool alive);
    uint8_t get_cell_last_generation_alive_neighbor_count(Grid_Point point);

    Button *find_button(Grid_Point point);
    void remove_door_at(Grid_Point point);

    void load(const char *level_name);
    void reload();
    void update();
    void draw();
};
