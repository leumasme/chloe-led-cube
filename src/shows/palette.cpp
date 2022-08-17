#include <FastLED.h>

#include "coords.h"

namespace PaletteBehavior {

int tickNum;
CRGBPalette16 palette;
TBlendType blending;

void SetupTotallyRandomPalette(CRGBPalette16* palette) {
    for (int i = 0; i < 16; i++) {
        *palette[i] = CHSV(random8(), 255, random8());
    }
}

void SetupBlackAndWhiteStripedPalette(CRGBPalette16* palette) {
    // 'black out' all 16 palette entries...
    fill_solid(*palette, 16, CRGB::Black);
    // and set every fourth one to white.
    *palette[0] = CRGB::White;
    *palette[4] = CRGB::White;
    *palette[8] = CRGB::White;
    *palette[12] = CRGB::White;
}

void SetupPurpleAndGreenPalette(CRGBPalette16* palette) {
    CRGB purple = CHSV(HUE_PURPLE, 255, 255);
    CRGB green = CHSV(HUE_GREEN, 255, 255);
    CRGB black = CRGB::Black;

    *palette =
        CRGBPalette16(green, green, black, black, purple, purple, black, black,
                      green, green, black, black, purple, purple, black, black);
}

const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM = {
    CRGB::Red,
    CRGB::Gray,  // 'white' is too bright compared to red and blue
    CRGB::Blue, CRGB::Black,

    CRGB::Red,  CRGB::Gray,  CRGB::Blue,  CRGB::Black,

    CRGB::Red,  CRGB::Red,   CRGB::Gray,  CRGB::Gray,
    CRGB::Blue, CRGB::Blue,  CRGB::Black, CRGB::Black};

void SetPalette(uint8_t paletteNum, CRGBPalette16* palette,
                TBlendType* blending) {
    switch (paletteNum) {
        case 0:
            *palette = RainbowColors_p;
            *blending = LINEARBLEND;
            break;
        case 1:
            *palette = RainbowStripeColors_p;
            *blending = NOBLEND;
            break;
        case 2:
            *palette = RainbowStripeColors_p;
            *blending = LINEARBLEND;
            break;
        case 3:
            SetupPurpleAndGreenPalette(palette);
            *blending = LINEARBLEND;
            break;
        case 4:
            SetupTotallyRandomPalette(palette);
            *blending = LINEARBLEND;
            break;
        case 5:
            SetupBlackAndWhiteStripedPalette(palette);
            *blending = NOBLEND;
            break;
        case 6:
            SetupBlackAndWhiteStripedPalette(palette);
            *blending = LINEARBLEND;
            break;
        case 7:
            *palette = CloudColors_p;
            *blending = LINEARBLEND;
            break;
        case 8:
            *palette = PartyColors_p;
            *blending = LINEARBLEND;
            break;
        case 9:
            *palette = myRedWhiteBluePalette_p;
            *blending = NOBLEND;
            break;
        case 10:
            *palette = myRedWhiteBluePalette_p;
            *blending = LINEARBLEND;
            break;
    }
}

void FillLEDsFromPaletteColors(uint8_t startIndex, CRGB leds[NUM_LEDS],
                               CRGBPalette16 palette, TBlendType blending) {
    uint8_t brightness = 255;

    for (int i = 0; i < NUM_LEDS; i++) {
        leds[i] = ColorFromPalette(palette, startIndex, brightness, blending);
        startIndex += 3;
    }
}

void start(int paletteNum) {
    Serial.println("[Palette] Starting palette " + String(paletteNum));
    tickNum = 0;
    SetPalette(paletteNum, &palette, &blending);
}

void tick(CRGB leds[NUM_LEDS]) {
    FillLEDsFromPaletteColors(tickNum, leds, palette, blending);
    tickNum++;
}

void stop() {}

}  // namespace PaletteBehavior