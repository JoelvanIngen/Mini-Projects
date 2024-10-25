//src/grid.h

#ifndef GRID_H
#define GRID_H

#define SNOWFALL_GRID_WIDTH 255
#define SNOWFALL_GRID_HEIGHT 140
#define SNOWFALL_GRID_SIZE (SNOWFALL_GRID_WIDTH * SNOWFALL_GRID_HEIGHT)

#include <stdint.h>

typedef enum CellColour {
    EMPTY,  // Black
    SNOW,   // White
} CellColour;

typedef struct Grid {
    CellColour colours[SNOWFALL_GRID_SIZE];
} Grid;

static int64_t getGridIndex(const int64_t x, const int64_t y) {
    return y * SNOWFALL_GRID_WIDTH + x;
}

Grid* gridCreate();
void gridDestroy(Grid** grid);

#endif //GRID_H
