#include <FastLED.h>

#include "coords.h"
#include "shows/matrix.h"
#include "shows/palette.h"
#include "shows/snake.h"

#define LED_PIN 2
#define LED_TYPE WS2812
#define COLOR_ORDER GRB
#define UPDATES_PER_SECOND 100

CRGB leds[NUM_LEDS];

// Touch sensor to switch between palettes.
int pinTouch = A2;
// Rotation potentiometer to control brightness
int pinBrightness = A0;

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

#define OTHER_SHOW_COUNT 2
void start_show(int showIndex) {
    showIndex = showIndex % (PALETTE_COUNT + OTHER_SHOW_COUNT);
    Serial.println("Switching to show/palette " + String(showIndex));
    if (showIndex >= PALETTE_COUNT) {
        // non-palette show
        switch (showIndex - PALETTE_COUNT) {
            case 0:
                MatrixBehavior::start();
                break;
            case 1:
                SnakeBehavior::start();
                break;
        }
    } else {
        // palette show
        PaletteBehavior::start(showIndex);
    }
}

void tick_show(int showIndex) {
    showIndex = showIndex % (PALETTE_COUNT + OTHER_SHOW_COUNT);
    if (showIndex >= PALETTE_COUNT) {
        // non-palette show
        switch (showIndex - PALETTE_COUNT) {
            case 0:
                MatrixBehavior::tick(leds);
                break;
            case 1:
                SnakeBehavior::tick(leds);
                break;
        }
    } else {
        // palette show
        PaletteBehavior::tick(leds);
    }
}

void loop() {
    static int currentPalette = -1;
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
            start_show(touchIncrement);

            // ensure we don't keep switching if we keep touching
            hasTouched = true;
        }
    } else if (isRunningPeriodicPalette) {
        int secondInMinute = (millis() / 1000) % 60;
        int nextPalette =
            (secondInMinute / 5) % (PALETTE_COUNT + OTHER_SHOW_COUNT);
        if (currentPalette != nextPalette) {
            currentPalette = nextPalette;
            start_show(nextPalette);
        }
    } else {
        // releasing touch -> a new touch can be accepted
        hasTouched = false;
    }

    tick_show(isRunningPeriodicPalette ? currentPalette : touchIncrement);

    int elapsed = millis() - before;
    Serial.println("Elapsed to tick: " + String(elapsed) + "ms");

    FastLED.show();
    FastLED.delay(1000 / UPDATES_PER_SECOND);
}