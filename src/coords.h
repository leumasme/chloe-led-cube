#include <FastLED.h>

#define CUBE_SIZE 5
#define NUM_LEDS CUBE_SIZE * CUBE_SIZE * CUBE_SIZE

CRGB* pixel_at_coords(CRGB leds[], int x, int y, int z);