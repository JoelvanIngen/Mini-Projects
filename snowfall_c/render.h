//src/render.h

#ifndef RENDER_H
#define RENDER_H


#include <stdbool.h>

#include "grid.h"

#define SNOWFALL_CELL_SIZE 10
#define WINDOW_WIDTH (SNOWFALL_GRID_WIDTH * SNOWFALL_CELL_SIZE)
#define WINDOW_HEIGHT (SNOWFALL_GRID_HEIGHT * SNOWFALL_CELL_SIZE)
#define WINDOW_TITLE "Snowfall"
#define WINDOW_FPS 30

struct Grid;

void snowfallInitWindow();
void snowfallDestroyWindow();
bool snowfallRenderReportWindowClose(const struct Grid* grid);

#endif //RENDER_H
