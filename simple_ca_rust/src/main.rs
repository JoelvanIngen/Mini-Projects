use crate::ca::entry;

mod ca;
mod parameters;
mod grid;
mod render;
mod math;

#[macroquad::main("CA")]
async fn main() {
    entry().await;
}
