// src/sim.cpp

#include <random>

#include "sim.h"
#include "render.h"

#include "grid.h"

std::random_device rd;
std::mt19937 gen(rd());
std::uniform_real_distribution<> dis(0.0, 1.0);

static double normalised_random() {
    return dis(gen);
}

static void melt_snowflakes(Grid* grid) {
    constexpr size_t row = SNOWFALL_GRID_HEIGHT - 1;

    for (size_t col = 0; col < SNOWFALL_GRID_WIDTH; col++) {
        if (grid->getCell(col, row) != SNOW) continue;
        if (normalised_random() < SNOW_MELT_CHANCE) grid->setCell(col, row, EMPTY);
    }
}

static void try_collapse_snowpile(Grid *grid, const size_t col, const size_t row) {
    if (grid->getCell(col + 1, row + 1) == EMPTY) {
        grid->setCell(col, row, EMPTY);
        grid->setCell(col + 1, row + 1, SNOW);
    } else if (grid->getCell(col - 1, row + 1) == EMPTY) {
        grid->setCell(col, row, EMPTY);
        grid->setCell(col - 1, row + 1, SNOW);
    }
}

static void try_move_single_snowflake(Grid* grid, const size_t col, const size_t row) {
    // Immediate return if not snow
    if (grid->getCell(col, row) == EMPTY) return;

    // If next row is occupied, try to collapse snowpile
    if (grid->getCell(col, row + 1) == SNOW) return try_collapse_snowpile(grid, col, row);

    // Move snowflake
    grid->setCell(col, row, EMPTY);
    grid->setCell(col, row + 1, SNOW);
}

static void move_snowflakes_single_row(Grid* grid, const size_t row) {
    for (size_t col = 0; col < SNOWFALL_GRID_WIDTH; col++) {
        try_move_single_snowflake(grid, col, row);
    }
}

static void move_snowflakes(Grid *grid) {
    // Move lowest rows first
    // Start with high value since grid grows downwards
    for (size_t row = SNOWFALL_GRID_HEIGHT - 2; row > 0; row--) {
        move_snowflakes_single_row(grid, row);
    }

    // Last iteration
    move_snowflakes_single_row(grid, 0);
}

static void spawn_snowflakes(Grid* grid) {
    for (size_t col = 0; col < SNOWFALL_GRID_WIDTH; col++) {
        if (normalised_random() < SNOW_SPAWN_CHANCE) grid->setCell(col, 0, SNOW);
    }
}

static void perform_single_step(Grid* grid) {
    melt_snowflakes(grid);
    move_snowflakes(grid);
    spawn_snowflakes(grid);
    render_snowflakes(grid);
}

static void sim_loop() {
    // Create grid
    Grid grid;

    while (!window_should_close()) {
        perform_single_step(&grid);
    }
}

void sim_entry() {
    // Init window
    init_window();

    // Simulation loop
    sim_loop();

    // Close window
    destroy_window();
}
