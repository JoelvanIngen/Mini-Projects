// src/grid.h

#ifndef GRID_H
#define GRID_H
#include <memory>

#define SNOWFALL_GRID_WIDTH 100
#define SNOWFALL_GRID_HEIGHT 80
#define SNOWFALL_GRID_SIZE (SNOWFALL_GRID_WIDTH * SNOWFALL_GRID_HEIGHT)

typedef enum CellState {
    EMPTY,
    SNOW,
} CellState;

class Grid {
    public:
    Grid() : grid(std::make_unique<CellState[]>(SNOWFALL_GRID_SIZE)) {
        // Initialise all as empty
        for (size_t i = 0; i < SNOWFALL_GRID_SIZE; i++) {
            grid[i] = EMPTY;
        }
    }

    void setCell(const size_t x, const size_t y, const CellState state) {
        grid[y * SNOWFALL_GRID_WIDTH + x % SNOWFALL_GRID_WIDTH] = state;
    }

    [[nodiscard]]
    CellState getCell(const size_t x, const size_t y) const {
        return grid[y * SNOWFALL_GRID_WIDTH + x];
    }

    private:
    std::unique_ptr<CellState[]> grid;
};

#endif //GRID_H
