//src/grid.c

#include "grid.h"

#include <stdlib.h>

#include "errorcode.h"

Grid* gridCreate() {
    Grid* grid = malloc(sizeof(Grid));
    if (grid == NULL) { exit(ERROR_GRID_MALLOC_FAIL); }

    // Initialise all grid squares as black
    for (int i = 0; i < SNOWFALL_GRID_SIZE; i++) {
        grid->colours[i] = EMPTY;
    }

    return grid;
}

void gridDestroy(Grid** grid) {
    free(*grid);
    *grid = NULL;
}
