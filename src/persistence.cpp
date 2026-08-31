#include "persistence.hpp"

constexpr const char *save_file_name = "game_progress.bin";

#if defined(PLATFORM_WEB)
    #include <emscripten/emscripten.h>

    int load_current_level_index()
    {
        return EM_ASM_INT({
            try {
                const value = window.localStorage.getItem(
                    "sokonway_current_level_index"
                );
                if (value === null) {
                    return 0;
                }
                const level = Number.parseInt(value, 10);
                return Number.isFinite(level) ? level : 0;
            } catch (error) {
                console.error(`Error loading the game progress: ${error}`);
                return 0;
            }
        });
    }

    void save_current_level_index(int level_index)
    {
        EM_ASM({
            try {
                window.localStorage.setItem(
                    "sokonway_current_level_index",
                    String($0)
                );
            } catch (error) {
                console.error(`Error saving the game progress: ${error}`);
            }
        }, level_index);
    }
#else
    #include <cstdio>

    int load_current_level_index()
    {
        FILE *file = fopen(save_file_name, "rb");
        if (file) {
            int result = 0;
            fread(&result, sizeof(result), 1, file);
            fclose(file);
            return result;
        }
        return 0;
    }

    void save_current_level_index(int level_index)
    {
        FILE *file = fopen(save_file_name, "wb");
        if (file) {
            fwrite(&level_index, sizeof(level_index), 1, file);
            fclose(file);
        }
    }
#endif