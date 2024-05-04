#include "common.h"

#define BALL_WIDTH (3)
#define INITIAL_SPEED (0.5)
#define ACCELERATION (0.001)

struct RollingBallData {
    float pos;
    float speed;
    int color;
};

RollingBallData rolling_ball_data;

void setup_rolling_ball() {
    rolling_ball_data = RollingBallData{
            LEDS_COUNT,
            INITIAL_SPEED,
            0,
    };
}

void rolling_ball() {
    for (int i = 0; i < LEDS_COUNT; i++) {
        strip.setLedColorData(i, 0, 0, 0);
    }
    for (int i = max((int) rolling_ball_data.pos - BALL_WIDTH, 0);
         i < min((int) rolling_ball_data.pos + BALL_WIDTH, LEDS_COUNT); i++) {
        strip.setLedColorData(i, strip.Wheel(rolling_ball_data.color));
    }

    rolling_ball_data.pos -= rolling_ball_data.speed;
    rolling_ball_data.speed += ACCELERATION;

    if (rolling_ball_data.pos < 0) {
        rolling_ball_data.pos = LEDS_COUNT;
        rolling_ball_data.speed = INITIAL_SPEED;
        rolling_ball_data.color = (rolling_ball_data.color + 180) % 256;
    }
}