#pragma once

#include <raylib.h>
#include <cstddef>
#include <cassert>

#define TILE_W 8
#define TILE_H 8

#define LEVEL_TILE_COLS 40
#define LEVEL_TILE_ROWS 22

static_assert(LEVEL_TILE_COLS > 0);
static_assert(LEVEL_TILE_ROWS > 0);

#define GAME_SCALE 2
#define SCREEN_W (TILE_W * LEVEL_TILE_COLS)
#define SCREEN_H (TILE_H * LEVEL_TILE_ROWS)

#define IsKeyPressedOrRepeat(key) (IsKeyPressed(key) || IsKeyPressedRepeat(key))

#define PICO8_BLACK Color{0, 0, 0, 255}
#define PICO8_DARKBLUE Color{29, 43, 83, 255}
#define PICO8_DARKPURPLE Color{126, 37, 83, 255}
#define PICO8_DARKGREEN Color{0, 135, 81, 255}
#define PICO8_BROWN Color{171, 82, 54, 255}
#define PICO8_DARKGREY Color{95, 87, 79, 255}
#define PICO8_LIGHTGREY Color{194, 195, 199, 255}
#define PICO8_WHITE Color{255, 241, 232, 255}
#define PICO8_RED Color{255, 0, 77, 255}
#define PICO8_ORANGE Color{255, 163, 0, 255}
#define PICO8_YELLOW Color{255, 236, 39, 255}
#define PICO8_GREEN Color{0, 228, 54, 255}
#define PICO8_BLUE Color{41, 173, 255, 255}
#define PICO8_LAVENDER Color{131, 118, 156, 255}
#define PICO8_PINK Color{255, 119, 168, 255}
#define PICO8_LIGHTPEACH Color{255, 204, 170, 255}

template <typename T, size_t N>
struct Fixed_Array {
    T data[N];
    size_t count = 0;

    const T& operator[](size_t i) const { return data[i]; }
          T& operator[](size_t i)       { return data[i]; }
    
    void append(const T &element)
    {
        assert(count + 1 <= N);
        data[count++] = element;
    }

    void unordered_remove(const T &element)
    {
        for (size_t i = 0; i < count; i++) {
            if (data[i] == element) {
                data[i] = data[count - 1];
                break;
            }
        }
    }
};

template <typename T>
struct Option {
    T value;
    bool has_value;
};