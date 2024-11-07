// src/render.h

#ifndef RENDER_H
#define RENDER_H

#include "grid.h"

#define SNOWFALL_CELL_SIZE 10
#define WINDOW_WIDTH (SNOWFALL_GRID_WIDTH * SNOWFALL_CELL_SIZE)
#define WINDOW_HEIGHT (SNOWFALL_GRID_HEIGHT * SNOWFALL_CELL_SIZE)
#define WINDOW_TITLE "Snowfall"
#define WINDOW_FPS 60

void init_window();
void destroy_window();
bool window_should_close();
void render_snowflakes(const Grid* grid);

#endif //RENDER_H
