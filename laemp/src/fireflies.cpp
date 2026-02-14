#include "field.h"
#include "figures.h"

#define NUM_FIREFLIES 20
#define BLINK_DURATION_TICKS ((int)round(0.5*FPS))
#define INFLUENCE_TICKS ((int)round(0.2*FPS))
#define MIN_DELAY_TICKS ((int)round(2*FPS))

struct Firefly {
    float x;
    float y;
    float vel_x;
    float vel_y;
    int change_vel;
    int internalTimeout;
    int lastBlink;
    int ticksTillNextBlink;
};

struct FireflyData {
    int tick;
    Firefly fireflies[NUM_FIREFLIES];
};

FireflyData fireflies_data;


float random_firefly_velocity() {
    return ((float) random(0, 100) - 50.f) / 150.f;
}

int random_firefly_duration() {
    return random(3, 10) * FPS;
}

void setup_fireflies() {
    fireflies_data.tick = 0;
    for (int i = 0; i < NUM_FIREFLIES; i++) {
        fireflies_data.fireflies[i] = Firefly{
            (float) random(0, FIELD_WIDTH),
            (float) random(0, FIELD_HEIGHT),
            random_firefly_velocity(),
            random_firefly_velocity(),
            random_firefly_duration(),
            random(5 * FPS, 8 * FPS),
            -BLINK_DURATION_TICKS,
            random(0 * FPS, 8 * FPS) + BLINK_DURATION_TICKS
        };
    }
}

void render_fireflies() {
    set_field_color(0);

    uint32_t hue = (fireflies_data.tick / 8) % 360;
    for (int i = 0; i < NUM_FIREFLIES; i++) {
        Firefly currentFireFly = fireflies_data.fireflies[i];
        int x_pos = (int) round(currentFireFly.x);
        int y_pos = (int) round(currentFireFly.y);
        uint32_t brightness = 0;
        uint32_t timeSinceLastBlink = fireflies_data.tick - currentFireFly.lastBlink;
        if (timeSinceLastBlink < BLINK_DURATION_TICKS) {
            brightness = round(100 * sin(map(timeSinceLastBlink, 0, BLINK_DURATION_TICKS, 0, PI)));
        }
        uint32_t color = strip.hsv2rgb(hue, 50, brightness);

        paint_figure_ywrapping(x_pos, y_pos, 1, 1, color, figure_bubble);
    }
}

void update_fireflies() {
    for (int i = 0; i < NUM_FIREFLIES; i++) {
        Firefly &currentFireFly = fireflies_data.fireflies[i];
        if (currentFireFly.change_vel <= fireflies_data.tick) {
            currentFireFly.change_vel = fireflies_data.tick + random_firefly_duration();
            currentFireFly.vel_x = random_firefly_velocity();
            currentFireFly.vel_y = random_firefly_velocity();
        }
        currentFireFly.x += currentFireFly.vel_x;
        currentFireFly.y += currentFireFly.vel_y;

        if (currentFireFly.lastBlink + currentFireFly.ticksTillNextBlink < fireflies_data.tick
            && currentFireFly.lastBlink + BLINK_DURATION_TICKS < fireflies_data.tick) {
            currentFireFly.lastBlink = fireflies_data.tick;
            currentFireFly.ticksTillNextBlink = currentFireFly.internalTimeout;
            for (int j = 0; j < NUM_FIREFLIES; j++) {
                if (i != j) {
                    fireflies_data.fireflies[j].ticksTillNextBlink = max(
                        MIN_DELAY_TICKS, fireflies_data.fireflies[j].ticksTillNextBlink - INFLUENCE_TICKS);
                }
            }
        }
    }
}

void fireflies() {
    fireflies_data.tick++;

    update_fireflies();

    render_fireflies();

    show_field_ywrapping();
}
