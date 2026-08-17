#include <raylib.h>
#include "common.h"
#include "pico8.h"

#if defined(PLATFORM_WEB)
    #include <emscripten/emscripten.h>
#endif

Game_State game = {};

void game_init()
{
    game.textureAtlas = LoadTexture("assets/images/sokonway-texture-atlas.png");
    game.renderTexture = LoadRenderTexture(SCREEN_W, SCREEN_H);

    game.levels[0].load("0");
}

void game_cleanup()
{
    UnloadTexture(game.textureAtlas);
    UnloadRenderTexture(game.renderTexture);
}

void game_update()
{
    BeginTextureMode(game.renderTexture);
        ClearBackground(PICO8_DARKGREY);
        game.levels[0].draw();
    EndTextureMode();

    BeginDrawing();
        ClearBackground(BLACK);
        Rectangle screen_source = {0, 0, (float)SCREEN_W, -(float)SCREEN_H};
        Rectangle screen_dest = {0, 0, (float)GetScreenWidth(), (float)GetScreenHeight()};
        DrawTexturePro(game.renderTexture.texture, screen_source, screen_dest, {0, 0}, 0, WHITE);
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
