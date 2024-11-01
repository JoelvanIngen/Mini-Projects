/* This module serves to prevent magic numbers from appearing in code
 * until I make these parameters settable during runtime */
use macroquad::color::{Color, BLACK, RED, WHITE};

pub const RULE: usize = 30;

pub const K: usize = 2;
pub const R: usize = 1;


pub const GRID_WIDTH: usize = 400;
pub const GRID_HEIGHT: usize = 200;
pub const CELL_SIZE: usize = 5;

pub const TARGET_FIXED_FPS: bool = true;
pub const FPS: f64 = 60.0;

pub const VAL_TO_COLOR: [Color; 3] = [WHITE, BLACK, RED];