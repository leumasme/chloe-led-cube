#include "coords.h"

namespace MatrixBehavior {

struct MatrixPoint {
    int x;
    int uy;
    int z;
    int speed;
};

MatrixPoint points[15] = {};

void start() {
    Serial.println("[Matrix] Starting");
    // clear points
    for (int i = 0; i < 15; i++) {
        points[i].x = 0;
        points[i].uy = 0;
        points[i].z = 0;
        points[i].speed = 0;
    }
}

void tick(CRGB leds[]) {
    for (int i = 0; i < NUM_LEDS; i++) {
        leds[i] = CRGB(0, 0, 0);
    }
    // iterate over points and draw them with a green trail
    // upwards while moving down
    for (int i = 0; i < 15; i++) {
        MatrixPoint& point = points[i];
        if (points[i].speed == 0) continue;
        int y = points[i].uy / 1000;
        for (int i = 0; i < 4; i++) {
            y++;
            if (y >= 0) {
                CRGB* pixel =
                    pixel_at_coords(leds, points[i].x, y, points[i].z);
                // TODO: maybe nicer colors with soft smoothing
                pixel->setRGB(0, 250 - i * 80, 0);
            }
        }

        // remove point if its fully out of view
        points[i].uy -= points[i].speed;
        if (points[i].uy < -5000) {
            points[i].speed = 0;
        }
    }

    // add a new point with 2% chance if there are less than 15 points
    if (random(100) < 2) {
        for (int i = 0; i < 15; i++) {
            if (points[i].speed == 0) {
                points[i].x = random(0, CUBE_SIZE);
                points[i].uy = CUBE_SIZE * 4000;
                points[i].z = random(0, CUBE_SIZE);
                points[i].speed = random(20, 50);
                break;
            }
        }
    }
}

void stop() {}

}  // namespace MatrixBehavior