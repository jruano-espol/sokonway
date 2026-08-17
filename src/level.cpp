#include "level.h"

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

void Level_Grid::load(const char *identifier)
{
    char *text = LoadFileText(TextFormat("assets/levels/%s.txt", identifier));
    std::vector<std::string> lines = split_lines(text);
    assert(lines.size() > 0);
    UnloadFileText(text);

    this->width = lines[0].size();
    this->height = lines.size();
    assert(width > 0 && height > 0);
    this->data = new uint8_t[width * height];

    for (size_t line_index = 0; line_index < lines.size(); line_index++) {
        assert(lines[line_index].size() == width);
        for (size_t char_index = 0; char_index < lines[line_index].size(); char_index++) {
            uint8_t number = lines[line_index][char_index] - '0';
            int row = line_index;
            int col = char_index;
            this->set(row, col, Tile_Kind{number});
        }
    }
}

void Level_Grid::draw()
{
    assert(data);
    for (int row = 0; row < height; row++) {
        for (int col = 0; col < width; col++) {
            Tile_Kind kind = at(row, col);
            draw_tile(kind, row, col);
        }
    }
}
