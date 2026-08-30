#include <algorithm>
#include <cstdio>
#include <raylib.h>
#include <raymath.h>
#include "animation.hpp"
#include "common.hpp"
#include "game_state.hpp"

#if defined(PLATFORM_WEB)
    #include <emscripten/emscripten.h>
#endif

Game_State game = {};
int g_key_repeat_timer = 0;

static void draw_grid(Vector2 offset, float game_screen_w, float game_screen_h)
{
    const Color color = {255, 255, 255, 20};

    const float tile_h = game_screen_h / LEVEL_TILE_ROWS;
    for (int row = 0; row < LEVEL_TILE_ROWS; row++) {
        const float y = offset.y + row * tile_h;
        DrawLineV({offset.x, y}, {offset.x + game_screen_w, y}, color);
    }
    const float tile_w = game_screen_w / LEVEL_TILE_COLS;
    for (int col = 0; col < LEVEL_TILE_COLS; col++) {
        const float x = offset.x + col * tile_w;
        DrawLineV({x, offset.y}, {x, offset.y + game_screen_h}, color);
    }
}

static void draw_top_bar_ui(const Level &current_level, Vector2 game_offset, float game_screen_h)
{
    const float tile_h = game_screen_h / LEVEL_TILE_ROWS;

    Font font = GetFontDefault();
    constexpr float tile_percentage = 0.8f;
    const float font_size = tile_h * tile_percentage;
    const float spacing = 1.0f;
    Vector2 pos = Vector2AddValue(game_offset, tile_h * (1.0f - tile_percentage));
    const char *text = nullptr;

    text = TextFormat("generation: %zu", current_level.generation);
    DrawTextEx(font, text, pos, font_size, spacing, WHITE);
    pos.x += font_size * spacing + MeasureTextEx(font, text, font_size, spacing).x;

    if (current_level.has_flag(Level_Flag::Simulation_Speed_Fast)) {
        text = "(fast)";
        DrawTextEx(font, text, pos, font_size, spacing, SKYBLUE);
        pos.x += font_size * spacing + MeasureTextEx(font, text, font_size, spacing).x;
    }
}

void game_init()
{
    game.textureAtlas = LoadTexture("assets/images/sokonway-texture-atlas.png");
    game.renderTexture = LoadRenderTexture(SCREEN_W, SCREEN_H);

    constexpr int last_level_index = 9;
    for (int i = 0; i <= last_level_index; i++) {
        char level_name[3];
        snprintf(level_name, sizeof(level_name), "%d",i);
        game.levels[i].load(level_name);
    }
#if 0
    game.current_level = last_level_index;
#else
    game.current_level = 0;
#endif

    game.animation_player = Animation::from(Tile_Kind::Player, true, 20);
    game.animation_portal = Animation::from(Tile_Kind::Portal, true, 15);
    game.animation_door = Animation::from(Tile_Kind::Door, true, 10);

    game.player.grid_position = game.levels[game.current_level].initial_player_position;
}

void game_cleanup()
{
    UnloadTexture(game.textureAtlas);
    UnloadRenderTexture(game.renderTexture);
}

void game_update()
{
    g_key_repeat_timer++;

    Level &current_level = game.levels[game.current_level];

    game.animation_player.update();
    game.animation_portal.update();
    game.animation_door.update();

    current_level.update();
    game.player.update();

    BeginTextureMode(game.renderTexture);
        ClearBackground(PICO8_DARKGREY);
        current_level.draw();
        game.player.draw();
    EndTextureMode();

    const float game_scale = std::min((float)GetScreenWidth()/SCREEN_W, (float)GetScreenHeight()/SCREEN_H);
    const float game_screen_w = SCREEN_W * game_scale;
    const float game_screen_h = SCREEN_H * game_scale;
    const Vector2 game_offset = {
        (GetScreenWidth() - game_screen_w) * 0.5f,
        (GetScreenHeight() - game_screen_h) * 0.5f,
    };

    BeginDrawing();
        ClearBackground(BLACK);
        Rectangle screen_source = {0, 0, (float)SCREEN_W, -(float)SCREEN_H};
        Rectangle screen_dest = {game_offset.x, game_offset.y, game_screen_w, game_screen_h};
        DrawTexturePro(game.renderTexture.texture, screen_source, screen_dest, {0, 0}, 0, WHITE);
        draw_grid(game_offset, game_screen_w, game_screen_h);
        draw_top_bar_ui(current_level, game_offset, game_screen_h);
    EndDrawing();
}

int main()
{
    constexpr int initial_scale = 2;
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(SCREEN_W * initial_scale, SCREEN_H * initial_scale, "Sokonway");

    SetExitKey(KEY_NULL);
    SetTargetFPS(60);

    game_init();

#if defined(PLATFORM_WEB)
    emscripten_set_main_loop(game_update, 0, 1);
#else
    while (!WindowShouldClose()) {
        game_update();
    }
#endif

    game_cleanup();
    CloseWindow();
    return 0;
}
