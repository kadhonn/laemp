#include "common.h"

int rolling_ball_tick = 0;

void setup_rolling_ball() {
    rolling_ball_tick = 0;
}

void rolling_ball() {
    int color = (rolling_ball_tick / LEDS_COUNT * 180) % 256;
    int j = (rolling_ball_tick % LEDS_COUNT);
    for (int i = 0; i < LEDS_COUNT; i++) {
        strip.setLedColorData(i, 0, 0, 0);
    }
    int pos = LEDS_COUNT - j * pow((float) j / (float) LEDS_COUNT, 0.5);
    for (int i = max(pos - 3, 0); i < min(pos + 3, LEDS_COUNT); i++) {
        strip.setLedColorData(i, strip.Wheel(color));
    }
}