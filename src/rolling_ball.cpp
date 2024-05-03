#include "common.h"

#define BALL_WIDTH (3)
#define INITIAL_SPEED (0.5)
#define ACCELERATION (0.001)

struct RollingBallData {
    float pos;
    float speed;
    int color;
};

RollingBallData rollingBallData;

void setup_rolling_ball() {
    rollingBallData = RollingBallData{
            LEDS_COUNT,
            INITIAL_SPEED,
            0,
    };
}

void rolling_ball() {
    for (int i = 0; i < LEDS_COUNT; i++) {
        strip.setLedColorData(i, 0, 0, 0);
    }
    for (int i = max((int) rollingBallData.pos - BALL_WIDTH, 0);
         i < min((int) rollingBallData.pos + BALL_WIDTH, LEDS_COUNT); i++) {
        strip.setLedColorData(i, strip.Wheel(rollingBallData.color));
    }

    rollingBallData.pos -= rollingBallData.speed;
    rollingBallData.speed += ACCELERATION;

    if (rollingBallData.pos < 0) {
        rollingBallData.pos = LEDS_COUNT;
        rollingBallData.speed = INITIAL_SPEED;
        rollingBallData.color = (rollingBallData.color + 180) % 256;
    }
}