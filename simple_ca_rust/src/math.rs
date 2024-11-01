use std::ops::{Add, Rem};

pub fn pos_mod<T>(val: T, modulo: T) -> T
where T: Rem<Output = T> + Add<Output = T> + Copy {
    // Ensures a positive result, as val % mod does not.
    ((val % modulo) + modulo) % modulo
}