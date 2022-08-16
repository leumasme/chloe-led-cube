#include "coords.h"

#include <FastLED.h>

CRGB* pixel_at_coords(CRGB leds[], int x, int y, int z) {
    int offset = z % 2 == 0 ? 0 : -2 * x - 1 + z * CUBE_SIZE;
    return &leds[x + z * CUBE_SIZE + y * CUBE_SIZE * CUBE_SIZE + offset];
}