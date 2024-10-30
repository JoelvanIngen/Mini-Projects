//src/main.rs

use crate::sim::sim_entry;

mod constants;
mod sim;
mod render;

#[macroquad::main("Snowfall")]
async fn main() {
    sim_entry().await;
}
