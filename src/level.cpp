#include "level.hpp"
#include "atlas.hpp"
#include "game_state.hpp"

#include <sstream>
#include <string>
#include <vector>

static std::vector<std::string> split_lines(const char *text)
{
    std::vector<std::string> lines;
    std::istringstream stream{text};

    std::string line;
    while (std::getline(stream, line)) {
        lines.push_back(std::move(line));
    }

    return lines;
}

static Grid_Point get_corresponding_door_point(int level_id, Grid_Point point)
{
    switch (level_id) {
    case 0:
        if (point == Grid_Point{3, 14}) {
            return Grid_Point{7, 10};
        }
    default:
        break;
    }
    assert(!"unreachable");
}

template <Tile_Kind kind>
const Animation &PropAnimated<kind>::get_animation()
{
    assert(animation_frame_count_of(kind) > 0);
    switch (kind) {
    case Tile_Kind::Player:
        return game.animation_player;
    case Tile_Kind::Portal:
        return game.animation_portal;
    case Tile_Kind::Door:
        return game.animation_door;
    default:
      break;
    }
    assert(!"unreachable");
}

template <Tile_Kind kind>
void PropAnimated<kind>::draw()
{
    const Animation &animation = get_animation();
    animation.draw_tiled(point);
}

Level::~Level() {
    if (data) {
        delete[] data;
        data = nullptr;
    }
    if (initial_data) {
        delete[] initial_data;
        initial_data = nullptr;
    }
}

Tile_Kind Level::at(int row, int col) const
{
    assert(in_bounds(row, col));
    uint8_t value = data[row * width + col];
    return Tile_Kind{value};
}

Tile_Kind Level::at(Grid_Point point) const
{
    return at(point.row, point.col);
}

void Level::set(Grid_Point point, Tile_Kind kind)
{
    assert(in_bounds(point));
    const uint8_t value = (uint8_t)kind;
    data[point.row * width + point.col] = value;
}

void Level::set_initial(Grid_Point point, Tile_Kind kind)
{
    switch (kind) {
    case Tile_Kind::Player:
        assert(!has_flag(Level_Flag::Has_Player));
        flags |= (uint8_t)Level_Flag::Has_Player;
        initial_player_position = point;
        break;
    case Tile_Kind::Door:
        doors.append({ .point = point });
        set(point, kind);
        break;
    case Tile_Kind::Button:
        buttons.append({
            .point = point,
            .corresponding_door_point = get_corresponding_door_point(id, point),
        });
        set(point, kind);
        break;
    case Tile_Kind::Portal:
        assert(!has_flag(Level_Flag::Has_Portal));
        flags |= (uint8_t)Level_Flag::Has_Portal;
        portal.point = point;
        break;
    default:
        set(point, kind);
        break;
    }
}

Button *Level::find_button(Grid_Point point)
{
    for (int i = 0; i < buttons.count; i++) {
        if (buttons[i].point == point) {
            return &buttons[i];
        }
    }
    return nullptr;
}

void Level::remove_door_at(Grid_Point point)
{
    set(point, Tile_Kind::Air);
    doors.unordered_remove({ .point = point });
}

void Level::load(const char *level_name)
{
    char *text = LoadFileText(TextFormat("assets/levels/%s.txt", level_name));
    std::vector<std::string> lines = split_lines(text);
    assert(lines.size() > 0);
    UnloadFileText(text);

    this->id = atoi(level_name);
    this->width = lines[0].size();
    this->height = lines.size();
    assert(width > 0 && height > 0);
    this->data = new uint8_t[width * height];
    this->initial_data = new uint8_t[width * height];

    for (size_t line_index = 0; line_index < lines.size(); line_index++) {
        assert(lines[line_index].size() == width);
        for (size_t char_index = 0; char_index < lines[line_index].size(); char_index++) {
            const uint8_t number = lines[line_index][char_index] - '0';
            const Grid_Point point = {
                .row = (int)line_index,
                .col = (int)char_index,
            };
            initial_data[point.row * width + point.col] = number;
            this->set_initial(point, Tile_Kind{number});
        }
    }
    assert(has_flag(Level_Flag::Has_Player));
    assert(has_flag(Level_Flag::Has_Portal));
}

void Level::draw()
{
    assert(data);
    for (int row = 0; row < height; row++) {
        for (int col = 0; col < width; col++) {
            Tile_Kind kind = at(row, col);
            draw_tile(kind, row, col, true);
        }
    }
    for (size_t i = 0; i < buttons.count; i++) {
        int atlas_index = atlas_index_from(Tile_Kind::Button);
        if (buttons[i].pressed) {
            atlas_index++;
        }
        draw_sprite(atlas_index, buttons[i].point);
    }
    for (size_t i = 0; i < doors.count; i++) {
        doors[i].draw();
    }
    portal.draw();
}
