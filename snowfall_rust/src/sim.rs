//src/sim.rs

use macroquad::time;
use rand::{thread_rng, Rng};
use rand::rngs::ThreadRng;
use crate::constants::{GRID_HEIGHT, GRID_SIZE, GRID_WIDTH, MELT_CHANCE};
use crate::render::render;

#[derive(Clone, PartialEq)]
pub enum CellState {
    Snow,
    Empty,
}

pub fn get_grid_index(x: usize, y: usize) -> usize {
    y * GRID_WIDTH + x
}

fn melt_snowflakes(grid: &mut Box<[CellState]>, rng: &mut ThreadRng) {
    let row: usize = GRID_HEIGHT - 1;

    for col in 0..GRID_WIDTH {
        let idx = get_grid_index(col, row);

        // Skip if cell is empty
        if grid[idx] == CellState::Empty { continue }

        // Skip if not random chance
        if rng.gen_range(0.0..1.0) > MELT_CHANCE { continue }

        grid[get_grid_index(col, row)] = CellState::Empty;
    }
}

fn try_snowpile_collapse(grid: &mut Box<[CellState]>, row: usize, col: usize) {
    let old_idx = get_grid_index(col, row);
    let new_row = row + 1;

    // Try right
    let new_col = (col + 1) % GRID_WIDTH;
    let new_idx = get_grid_index(new_col, new_row);
    if grid[new_idx] == CellState::Empty {
        grid[new_idx] = CellState::Snow;
        grid[old_idx] = CellState::Empty;
        return
    }

    // Try left
    // Conversion hack to prevent sub overflow
    let new_col = (col as isize - 1) as usize % GRID_WIDTH;
    let new_idx = get_grid_index(new_col, new_row);
    if grid[new_idx] == CellState::Empty {
        grid[new_idx] = CellState::Snow;
        grid[old_idx] = CellState::Empty;
        return
    }
}

fn try_move_single_snowflake(grid: &mut Box<[CellState]>, row: usize, col: usize) {
    let old_idx = get_grid_index(col, row);

    // Immediate return if index doesn't contain snowflake
    if grid[old_idx] == CellState::Empty { return }

    let new_idx = get_grid_index(col, row + 1);

    // Early return if next index is already a snowflake
    if grid[new_idx] == CellState::Snow { return try_snowpile_collapse(grid, row, col); }

    grid[new_idx] = CellState::Snow;
    grid[old_idx] = CellState::Empty;
}

fn move_snowflakes(grid: &mut Box<[CellState]>) {
    // Skip last row as snow cannot fall further than that
    for row in (0..GRID_HEIGHT - 1).rev() {
        for col in 0..GRID_WIDTH {
            try_move_single_snowflake(grid, row, col);
        }
    }
}

fn try_append_snowflake(grid: &mut Box<[CellState]>, col: usize) {
    if grid[col] == CellState::Empty {
        grid[col] = CellState::Snow;
    }
}

fn new_snowflakes(grid: &mut Box<[CellState]>, rng: &mut ThreadRng) {
    for col in 0..GRID_WIDTH {
        if rng.gen_range(0.0..1.0) < MELT_CHANCE {
            try_append_snowflake(grid, col);
        }
    }
}

fn single_sim_step(grid: &mut Box<[CellState]>, rng: &mut ThreadRng) {
    melt_snowflakes(grid, rng);
    move_snowflakes(grid);
    new_snowflakes(grid, rng);
}

fn sleep(time_s: f64) {
    std::thread::sleep(std::time::Duration::from_secs_f64(time_s));
}

async fn sim_loop(grid: &mut Box<[CellState]>, rng: &mut ThreadRng) {
    let target_frame_time = 1.0 / 60.0;

    loop {
        let start_time = time::get_time();

        single_sim_step(grid, rng);
        render(grid).await;

        let frame_time = time::get_time() - start_time;
        if frame_time < target_frame_time {
            let delay = target_frame_time - frame_time;
            sleep(delay);
        }
    }
}

pub async fn sim_entry() {
    // Create grid
    let mut grid = vec![CellState::Empty; GRID_SIZE].into_boxed_slice();

    // Create random generator
    let mut rng = thread_rng();

    sim_loop(&mut grid, &mut rng).await;
}