#include <FastLed.h>

#include "coords.h"
#define SNAKE_MOVE_EVERY 75

namespace SnakeBehavior {

int remaining_ticks = 0;
int head_id = 1;
int snake_length = 1;
int grid[NUM_LEDS];
int head_x, head_y, head_z;
int food_x, food_y, food_z;

enum PixelType { EMPTY, HEAD, BODY, FOOD };

PixelType get_type_from_value(int value) {
    if (value == -1) return FOOD;
    if (value == head_id) return HEAD;
    if (value - head_id < snake_length) return BODY;
    return EMPTY;
}

void place_food() {
    // this could get stuck, but it probably won't
    while (get_type_from_value(
               grid[index_from_coords(food_x, food_y, food_z)]) != EMPTY) {
        food_x = random(0, CUBE_SIZE);
        food_y = random(0, CUBE_SIZE);
        food_z = random(0, CUBE_SIZE);
        Serial.println("Trying to place food at " + String(food_x) + ", " +
                       String(food_y) + ", " + String(food_z));
    }
    Serial.println("Placed food!");
    grid[index_from_coords(food_x, food_y, food_z)] = -1;
}

void start() {
    Serial.println("[Snake] Starting");
    // clear grid and reset
    for (int i = 0; i < NUM_LEDS; i++) {
        grid[i] = 0;
    }
    snake_length = 1;
    head_id = 1;
    remaining_ticks = SNAKE_MOVE_EVERY;

    // choose random start point of snake and set it to 1
    head_x = random(0, CUBE_SIZE);
    head_y = random(0, CUBE_SIZE);
    head_z = random(0, CUBE_SIZE);
    int head_index = index_from_coords(head_x, head_y, head_z);
    grid[head_index] = head_id;

    // choose random food point and set it to -1
    place_food();
}
void tick(CRGB leds[]) {
    remaining_ticks--;

    if (remaining_ticks == 0) {
        remaining_ticks = SNAKE_MOVE_EVERY;

        // find next head position in direction of the food
        if (food_x != head_x) {
            head_x += food_x > head_x ? 1 : -1;
        } else if (food_y != head_y) {
            head_y += food_y > head_y ? 1 : -1;
        } else if (food_z != head_z) {
            head_z += food_z > head_z ? 1 : -1;
        }

        int head_index = index_from_coords(head_x, head_y, head_z);
        PixelType next_pixel_type = get_type_from_value(grid[head_index]);
        if (next_pixel_type == BODY) {
            // game over
            // TODO: Loss animation
            Serial.println("Game over! Length: " + String(snake_length));
            start();
            return;
        }
        if (next_pixel_type == FOOD) {
            // food eaten
            snake_length++;
            Serial.println("Eaten food!");
            place_food();
        }

        grid[head_index] = ++head_id;

        // draw to cube
        for (int i = 0; i < NUM_LEDS; i++) {
            CRGB* pixel = pixel_at_coords(
                leds, i % CUBE_SIZE, i / CUBE_SIZE / CUBE_SIZE, i / CUBE_SIZE);
            switch (get_type_from_value(grid[i])) {
                case EMPTY:
                    pixel->setRGB(0, 0, 0);
                    break;
                case HEAD:
                    pixel->setRGB(100, 200, 100);
                    break;
                case BODY:
                    pixel->setRGB(50, 100, 50);
                    break;
                case FOOD:
                    pixel->setRGB(0, 255, 0);
                    break;
            }
        }
    }
}
void stop() {}

}  // namespace SnakeBehavior