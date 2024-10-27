//src/sim.c

#include "sim.h"

#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

#include "grid.h"
#include "render.h"

/* If a snowflake is on top of a pile, it will try to move downwards diagonally toward the right
 * This should prevent small noisy piles from forming
 * If this would cross the right grid boundary, will loop to the start (left)
 * If the right side is occupied too, will try left following the same logic */
static void trySnowpileCollapse(Grid* grid, const int64_t row, const int64_t col, const int64_t idx) {
    const int64_t newRow = row + 1;

    // Immediate return if slowflake is already on the ground
    if (newRow >= SNOWFALL_GRID_HEIGHT) return;

    // Loopbacks if column exceeds valid grid columns on either side
    const int64_t newColumnRight = (col + 1) % SNOWFALL_GRID_WIDTH;
    const int64_t newColumnLeft = (col - 1) % SNOWFALL_GRID_WIDTH;

    const int64_t newIndexRight = getGridIndex(newColumnRight, newRow);
    const int64_t newIndexLeft = getGridIndex(newColumnLeft, newRow);

    // Try both new options and apply if possible
    if (grid->colours[newIndexRight] == EMPTY) {
        grid->colours[newIndexRight] = SNOW;
        grid->colours[idx] = EMPTY;
    } else if (grid->colours[newIndexLeft] == EMPTY) {
        grid->colours[newIndexLeft] = SNOW;
        grid->colours[idx] = EMPTY;
    }
}

static void tryMoveSingleSnowflake(Grid* grid, const int64_t row, const int64_t col) {
    const int64_t idx = getGridIndex(col, row);

    // Immediate return if array at idx does not contain snowflake
    if (grid->colours[idx] != SNOW) return;

    // Addition because the grid row index grows downward
    const int64_t newIndex = idx + SNOWFALL_GRID_WIDTH;

    // Early return if next index would be out of bounds
    if (newIndex >= SNOWFALL_GRID_SIZE) return;

    // Prevent pile of snow if it is on another snowflake
    if (grid->colours[newIndex] == SNOW) return trySnowpileCollapse(grid, row + 1, col, idx);
    
    grid->colours[newIndex] = SNOW;
    grid->colours[idx] = EMPTY;
}

static void moveRow(Grid* grid, const int64_t row) {
    for (int64_t col = 0; col < SNOWFALL_GRID_WIDTH; col++) {
        tryMoveSingleSnowflake(grid, row, col);
    }
}

static void moveSnowflakes(Grid* grid) {
    // Start at bottom such that falling snowflakes create space for snowflakes directly above
    for (int64_t row = SNOWFALL_GRID_HEIGHT - 1; row >= 0; row--) {
        moveRow(grid, row);
    }
}

static double normaliseRandom() {
    return (double) rand() / (double) RAND_MAX; // NOLINT(cert-msc30-c, cert-msc50-cpp)
}

static void tryAppendSnowflake(Grid* grid, const int64_t idx) {
    if (grid->colours[idx] == SNOW) { return; }
    grid->colours[idx] = SNOW;
}

static void newSnowflakes(Grid* grid) {
    for (int64_t col = 0; col < SNOWFALL_GRID_WIDTH; col++) {
        const double random = normaliseRandom();
        if (random < 0.1) {
            // Index = col because row = 0, no conversion necessary
            tryAppendSnowflake(grid, col);
        }
    }
}

/* Runs an iteration step and reports whether the window loop was terminated */
static bool singleStepReportWindowClose(Grid* grid) {
    moveSnowflakes(grid);
    newSnowflakes(grid);
    return snowfallRenderReportWindowClose(grid);
}

static void simLoop(Grid* grid) {
    srand(time(0));

    while (!singleStepReportWindowClose(grid)) {
        /* Nothing */
    }
}

void simEntry() {
    // Create grid
    Grid* grid = gridCreate();

    snowfallInitWindow();

    simLoop(grid);

    snowfallDestroyWindow();

    // Free grid
    gridDestroy(&grid);
}
