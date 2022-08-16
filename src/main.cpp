#include <FastLED.h>

#include "coords.h"
#include "shows/matrix.h"
#include "shows/palette.h"

#define LED_PIN 2
#define LED_TYPE WS2812
#define COLOR_ORDER GRB
#define UPDATES_PER_SECOND 100

CRGB leds[NUM_LEDS];

// Touch sensor to switch between palettes.
int pinTouch = A2;
// Rotation potentiometer to control brightness
int pinBrightness = A0;

CRGBPalette16 currentPalette;
TBlendType currentBlending;

uint8_t touchIncrement = 0;
bool isRunningPeriodicPalette = true;
bool hasTouched = false;

void setup() {
    delay(3000);  // power-up safety delay
    FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS)
        .setCorrection(TypicalLEDStrip);

    pinMode(pinBrightness, INPUT);
    pinMode(pinTouch, INPUT);

    PaletteBehavior::start(0);

    Serial.begin(9600);
}

void loop() {
    int brightness = map(analogRead(pinBrightness), 0, 1023, 0, 255);
    FastLED.setBrightness(brightness);

    // Check if human touched! If so we can switch from periodic palette
    int before = millis();
    int touchState = digitalRead(pinTouch);
    if (touchState == HIGH) {
        if (!hasTouched) {
            // Value is high and we're running periodic palette, so we can
            // switch to touch based until restart
            isRunningPeriodicPalette = false;
            touchIncrement += 1;
            PaletteBehavior::start(touchIncrement % (PALETTE_COUNT + 1));

            // ensure we don't keep switching if we keep touching
            hasTouched = true;
        }
    } else if (isRunningPeriodicPalette) {
        int secondInMinute = (millis() / 1000) % 60;
        static int previousSecond = -1;
        if (secondInMinute != previousSecond) {
            previousSecond = secondInMinute;
            PaletteBehavior::start((secondInMinute / 5) % (PALETTE_COUNT + 1));
        }
    } else {
        // releasing touch -> a new touch can be accepted
        hasTouched = false;
    }

    PaletteBehavior::tick(leds);

    int elapsed = millis() - before;
    Serial.println("Elapsed to tick: " + String(elapsed) + "ms");

    FastLED.show();
    FastLED.delay(1000 / UPDATES_PER_SECOND);
}