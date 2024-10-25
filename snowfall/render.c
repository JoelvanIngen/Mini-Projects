//src/render.c

#include "render.h"

#include <raylib.h>

#include "grid.h"

static Color SNOWFLAKE_COLOUR_TO_RAYLIB_COLOUR[] = {BLACK, WHITE};

void snowfallInitWindow() {
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE);
    SetTargetFPS(WINDOW_FPS);
}

void snowfallDestroyWindow() {
    CloseWindow();
}

static void drawSingleSnowflake(const Grid* grid, const int64_t row, const int64_t col) {
    const int64_t idx = getGridIndex(col, row);
    DrawRectangle((int) col * SNOWFALL_CELL_SIZE,
        (int) row * SNOWFALL_CELL_SIZE,
        SNOWFALL_CELL_SIZE,
        SNOWFALL_CELL_SIZE,
        SNOWFLAKE_COLOUR_TO_RAYLIB_COLOUR[grid->colours[idx]]);
}

static void drawSnowflakes(const Grid* grid) {
    for (int64_t row = 0; row < SNOWFALL_GRID_HEIGHT; row++) {
        for (int64_t col = 0; col < SNOWFALL_GRID_WIDTH; col++) {
            drawSingleSnowflake(grid, row, col);
        }
    }
}

bool snowfallRenderReportWindowClose(const Grid* grid) {
    BeginDrawing();
    ClearBackground(SKYBLUE);
    drawSnowflakes(grid);
    EndDrawing();
    return WindowShouldClose();
}
