use macroquad::prelude::*;
use crate::grid::Grid;
use crate::parameters::{CELL_SIZE, GRID_HEIGHT, GRID_WIDTH, VAL_TO_COLOR};

pub fn init() {
    request_new_screen_size((GRID_WIDTH * CELL_SIZE) as f32,
                            (GRID_HEIGHT * CELL_SIZE) as f32);
}

fn draw_cell(x: usize, y: usize, val: u8) {
    draw_rectangle(
        x as f32,
        y as f32,
        CELL_SIZE as f32,
        CELL_SIZE as f32,
        VAL_TO_COLOR[val as usize],
    )
}

pub async fn render(grid: &Grid) {
    clear_background(WHITE);
    
    let mut x: usize;
    let mut y: usize;
    for (i, row) in grid.get_rows().iter().enumerate() {
        y = i * CELL_SIZE;
        for (j, col) in row.iter().enumerate() {
            x = j * CELL_SIZE;
            draw_cell(x, y, *col);
        }
    }

    next_frame().await;
}