//src/render.rs

use macroquad::prelude::*;
use crate::constants::{CELL_SIZE, GRID_HEIGHT, GRID_WIDTH};
use crate::sim::{get_grid_index, CellState};
use crate::sim::CellState::Snow;

fn draw_single_snowflake(col: usize, row: usize) {
    draw_rectangle((col * CELL_SIZE) as f32,
                   (row * CELL_SIZE) as f32,
                   CELL_SIZE as f32,
                   CELL_SIZE as f32,
                   WHITE
    )
}

pub async fn render(grid: &mut Box<[CellState]>) {
    request_new_screen_size((GRID_WIDTH * CELL_SIZE) as f32,
                            (GRID_HEIGHT * CELL_SIZE) as f32);
    clear_background(BLACK);

    for row in 0..GRID_HEIGHT {
        for col in 0..GRID_WIDTH {
            if grid[get_grid_index(col, row)] == Snow {
                draw_single_snowflake(col, row);
            }
        }
    }

    next_frame().await;
}