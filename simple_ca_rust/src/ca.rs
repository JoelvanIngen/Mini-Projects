use std::collections::HashMap;
use std::io::{stdout, Write};
use std::thread::sleep;
use std::time::Duration;
use macroquad::time;
use crate::grid::Grid;
use crate::parameters::*;
use crate::render::{init, render};

fn base_10_to_base_k(mut num: usize, k: usize, r: usize) -> Box<[u8]> {
    let mut digits = Vec::new();

    while num > 0 {
        digits.push((num % k) as u8);
        num /= k;
    }

    digits.reverse();
    digits.into_boxed_slice()
}

fn base_k_to_base_10(num: Box<[u8]>, k: usize) -> usize {
    num.iter()
        .enumerate()
        .map(|(i, &digit)| (digit as usize) * k.pow((num.len() - 1 - i) as u32))
        .sum()
}

fn pad_zeros(k_repr: Box<[u8]>, n: usize) -> Box<[u8]> {
    // Pads zeroes on the left of a vec such that the amount n of digits is reached
    let mut res: Box<[u8]> = vec![0u8; n].into_boxed_slice();
    
    let mut res_idx = n;

    for k_idx in (0..k_repr.len()).rev() {
        res_idx -= 1;
        res[res_idx] = k_repr[k_idx];
    }

    res
}

fn create_rules(rule: usize, k: usize, r: usize) -> HashMap<Box<[u8]>, u8> {
    /* Creates a ruleset
    * - rule: chosen rule number for the simulation
    * - k: base of numbers
    * - r: neighbours or 'range'. Excludes middle */

    // Precompute sizes
    let neighbourhood_size = 2 * r + 1;
    let unique_states = k.pow(neighbourhood_size as u32);
    let max_rule = k.pow(unique_states as u32);

    // Base k representation of the rule
    // This will be the output for every possible state
    let rule_repr = pad_zeros(base_10_to_base_k(rule, k, r), unique_states);

    // Create hashmap that to look up states to find the next state according to chosen rule
    let mut state_to_next: HashMap<Box<[u8]>, u8> = HashMap::with_capacity(max_rule);

    // Populate hashmap
    for state_i in 0..unique_states {
        let rule_base_k = pad_zeros(base_10_to_base_k(state_i, k, r), neighbourhood_size);
        state_to_next.insert(rule_base_k.clone(), rule_repr[unique_states - state_i - 1]);

        let rpr = rule_repr[unique_states - state_i - 1];
        println!("state_i {state_i} | rule_base_k {rule_base_k:?} | rule_repr {rpr}");
    }

    state_to_next
}

fn single_iteration(grid: &mut Grid, ruleset: &HashMap<Box<[u8]>, u8>, r: usize) {
    grid.new_iteration();
    
    for col in 0..grid.width() {
        let prev = grid.get_slice_from_prev(col, r);
        // println!("col: {}, prev: {:?}", col, prev);
        stdout().flush().unwrap();
        let new = ruleset.get(&prev).unwrap();
        grid.set_value(col, *new)
    }
}

fn wait_for_frame(start_time: f64, target_frame_time: f64) {
    if TARGET_FIXED_FPS return;
    
    let end_time = time::get_time();
    
    let elapsed_time = end_time - start_time;
    
    if elapsed_time < target_frame_time {
        sleep(Duration::from_secs_f64(target_frame_time - elapsed_time));
    }
}

async fn start_loop() {
    let mut grid = Grid::new(GRID_WIDTH, GRID_HEIGHT);
    grid.activate_middle();
    
    let ruleset = create_rules(RULE, K, R);
    
    let target_frame_time = 1.0 / FPS;

    loop {
        let start_time = time::get_time();
        single_iteration(&mut grid, &ruleset, R);
        wait_for_frame(start_time, target_frame_time);
        render(&grid).await;
    }
}

fn check_parameter_validity(rule: usize, k: usize, r: usize, width: usize) {
    let neighbourhood_size = 2 * r + 1;
    let unique_states = k.pow(neighbourhood_size as u32);
    let max_rule = k.pow(unique_states as u32);
    
    if rule > max_rule {
        panic!("Rule {rule} is greater than maximum of {max_rule}!");
    }
    
    if neighbourhood_size > width {
        panic!("Neighbourhood size of {neighbourhood_size} is greater than grid width of {width}!");
    }
}

pub async fn entry() {
    check_parameter_validity(RULE, K, R, GRID_WIDTH);
    init();
    start_loop().await;
}