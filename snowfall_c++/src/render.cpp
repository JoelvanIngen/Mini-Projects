// src/render.cpp

#include <raylib.h>

#include "render.h"

static Color SNOWFLAKE_COLOUR_TO_RAYLIB_COLOUR[] = {BLACK, WHITE};

void init_window() {
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Hello World");
    SetTargetFPS(WINDOW_FPS);
}

void destroy_window() {
    CloseWindow();
}

bool window_should_close() {
    return WindowShouldClose();
}

static void draw_single_snowflake(const Grid* grid, const size_t col, const size_t row) {
    if (grid->getCell(col, row) == EMPTY) return;

    DrawRectangle(
        static_cast<int>(col * SNOWFALL_CELL_SIZE),
        static_cast<int>(row * SNOWFALL_CELL_SIZE),
        SNOWFALL_CELL_SIZE,
        SNOWFALL_CELL_SIZE,
        SNOWFLAKE_COLOUR_TO_RAYLIB_COLOUR[grid->getCell(col, row)]
    );
}

static void draw_snowflakes(const Grid* grid) {
    for (size_t row = 0; row < SNOWFALL_GRID_HEIGHT; row++) {
        for (size_t col = 0; col < SNOWFALL_GRID_WIDTH; col++) {
            draw_single_snowflake(grid, col, row);
        }
    }
}

void render_snowflakes(const Grid *grid) {
    BeginDrawing();
    ClearBackground(BLACK);
    draw_snowflakes(grid);
    EndDrawing();
}

