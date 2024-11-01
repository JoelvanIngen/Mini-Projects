use crate::math::pos_mod;

pub struct Grid {
    cells: Box<[Box<[u8]>]>,
    width: usize,
    height: usize,
    row_index: usize,
    prev_row: usize,
}

impl Grid {
    pub fn new(width: usize, height: usize) -> Grid {
        Grid {
            cells: vec![vec![0u8; width].into_boxed_slice(); height].into_boxed_slice(),
            width,
            height,
            row_index: 0,
            prev_row: 0,
        }
    }
    
    pub fn activate_middle(&mut self) {
        self.cells[0][self.width / 2] = 1;
    }
    
    pub fn width(&self) -> usize { self.width }
    
    pub fn height(&self) -> usize { self.height }
    
    pub fn get_slice_from_prev(&self, col: usize, r: usize) -> Box<[u8]> {
        (col as isize - r as isize .. col as isize + r as isize + 1)
            .map(|i| self.cells[self.prev_row][pos_mod(i, self.width as isize) as usize])
            .collect::<Vec<_>>()
            .into_boxed_slice()
        
    }
    
    pub fn set_value(&mut self, col: usize, value: u8) {
        self.cells[self.row_index][col] = value;
    }
    
    pub fn new_iteration(&mut self) {
        self.prev_row = self.row_index;
        self.row_index = (self.row_index + 1) % self.height;
    }
    
    pub fn get_rows(&self) -> &[Box<[u8]>] {
        &self.cells
    }
}