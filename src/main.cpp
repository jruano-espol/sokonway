#include <raylib.h>
#include "animation.hpp"
#include "common.hpp"
#include "game_state.hpp"

#if defined(PLATFORM_WEB)
    #include <emscripten/emscripten.h>
#endif

Game_State game = {};

static void draw_grid()
{
    const Color color = {255, 255, 255, 20};

    const int tile_h = GetScreenHeight() / LEVEL_TILE_ROWS;
    for (int row = 0; row < LEVEL_TILE_ROWS; row++) {
        int y = row * tile_h;
        DrawLine(0, y, GetScreenWidth(), y, color);
    }
    const int tile_w = GetScreenWidth() / LEVEL_TILE_COLS;
    for (int col = 0; col < LEVEL_TILE_COLS; col++) {
        int x = col * tile_w;
        DrawLine(x, 0, x, GetScreenHeight(), color);
    }
}

void game_init()
{
    game.textureAtlas = LoadTexture("assets/images/sokonway-texture-atlas.png");
    game.renderTexture = LoadRenderTexture(SCREEN_W, SCREEN_H);

    game.levels[0].load("0");
    game.current_level = 0;

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
    game.animation_player.update();
    game.animation_portal.update();
    game.animation_door.update();

    game.player.update();

    BeginTextureMode(game.renderTexture);
        ClearBackground(PICO8_DARKGREY);
        game.levels[0].draw();
        game.player.draw();
    EndTextureMode();

    BeginDrawing();
        ClearBackground(BLACK);
        Rectangle screen_source = {0, 0, (float)SCREEN_W, -(float)SCREEN_H};
        Rectangle screen_dest = {0, 0, (float)GetScreenWidth(), (float)GetScreenHeight()};
        DrawTexturePro(game.renderTexture.texture, screen_source, screen_dest, {0, 0}, 0, WHITE);
        draw_grid();
    EndDrawing();
}

int main()
{
    InitWindow(SCREEN_W * GAME_SCALE, SCREEN_H * GAME_SCALE, "Sokonway");
    game_init();

#if defined(PLATFORM_WEB)
    emscripten_set_main_loop(game_update, 0, 1);
#else
    SetExitKey(KEY_NULL);
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        game_update();
    }
#endif

    game_cleanup();
    CloseWindow();
    return 0;
}
