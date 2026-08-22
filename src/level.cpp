#include "level.hpp"
#include "atlas.hpp"

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

Tile_Kind Level::at(int row, int col)
{
    assert(in_bounds(row, col));
    uint8_t value = data[row * width + col];
    return Tile_Kind{value};
}

void Level::set(int row, int col, Tile_Kind kind)
{
    assert(in_bounds(row, col));
    switch (kind) {
    case Tile_Kind::Player: {
        initial_player_position = Grid_Point{row, col};
    } break;
    default: {
        uint8_t value = (uint8_t)kind;
        data[row * width + col] = value;
    } break;
    }
}

void Level::load(const char *identifier)
{
    char *text = LoadFileText(TextFormat("assets/levels/%s.txt", identifier));
    std::vector<std::string> lines = split_lines(text);
    assert(lines.size() > 0);
    UnloadFileText(text);

    this->width = lines[0].size();
    this->height = lines.size();
    assert(width > 0 && height > 0);
    this->data = new uint8_t[width * height];
    this->initial_data = new uint8_t[width * height];

    for (size_t line_index = 0; line_index < lines.size(); line_index++) {
        assert(lines[line_index].size() == width);
        for (size_t char_index = 0; char_index < lines[line_index].size(); char_index++) {
            uint8_t number = lines[line_index][char_index] - '0';
            int row = line_index;
            int col = char_index;
            initial_data[row * width + col] = number;
            this->set(row, col, Tile_Kind{number});
        }
    }
}

void Level::draw()
{
    assert(data);
    for (int row = 0; row < height; row++) {
        for (int col = 0; col < width; col++) {
            Tile_Kind kind = at(row, col);
            draw_tile(kind, row, col);
        }
    }
}
