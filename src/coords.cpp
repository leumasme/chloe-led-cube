#include "coords.h"

#include <FastLED.h>

// offset applied as index walks backward at the end of the cube
int index_from_coords(int x, int y, int z) {
    int offset = z % 2 == 0 ? 0 : -2 * x - 1 + z * CUBE_SIZE;
    return x + z * CUBE_SIZE + y * CUBE_SIZE * CUBE_SIZE + offset;
}

// reverse index_from_coords
void coords_from_index(int index, int* x, int* y, int* z) {
    *y = index / (CUBE_SIZE * CUBE_SIZE);
    index -= *y * CUBE_SIZE * CUBE_SIZE;
    *z = index / CUBE_SIZE;
    *x = *z % 2 == 0 ? index - *z * CUBE_SIZE : (*z + 1) * CUBE_SIZE - index - 1;
}

CRGB* pixel_at_coords(CRGB leds[], int x, int y, int z) {
    return &leds[index_from_coords(x, y, z)];
}