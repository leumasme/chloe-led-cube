#include <FastLED.h>
#define PALETTE_COUNT 10

namespace PaletteBehavior {

void start(int palette);
void tick(CRGB leds[]);
void stop();

}  // namespace PaletteBehavior