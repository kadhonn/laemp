#include "figures.h"

#define NUM_HEARTS 4

struct Heart {
    float x;
    float y;
    float vel_x;
    float vel_y;
    int change_vel;
    int size;
};

struct HeartsData {
    int tick;
    Heart hearts[NUM_HEARTS];
};

HeartsData hearts_data;

float randomVelocity() {
    return ((float) random(0, 100) - 50.f) / 150.f;
}

int randomDuration() {
    return random(3, 10) * FPS;
}

void setup_hearts() {
    hearts_data.tick = 0;
    for (int i = 0; i < NUM_HEARTS; i++) {
        hearts_data.hearts[i] = Heart{
                (float) random(0, FIELD_WIDTH),
                (float) random(0, FIELD_HEIGHT),
                randomVelocity(),
                randomVelocity(),
                randomDuration(),
                random(3, 6)
        };
    }
}

void render_hearts(int hue) {
    uint32_t heart_color = strip.Wheel(strip.hsv2rgb(hue, 100, 100));
    for (int i = 0; i < NUM_HEARTS; i++) {
        int x_pos = (int) round(hearts_data.hearts[i].x);
        int y_pos = (int) round(hearts_data.hearts[i].y);
        int size = hearts_data.hearts[i].size;

        paint_figure_ywrapping(x_pos, y_pos, size, size, heart_color, figure_heart);
    }
}

void update_hearts() {
    for (int i = 0; i < NUM_HEARTS; i++) {
        if (hearts_data.hearts[i].change_vel <= hearts_data.tick) {
            hearts_data.hearts[i].change_vel = hearts_data.tick + randomDuration();
            hearts_data.hearts[i].vel_x = randomVelocity();
            hearts_data.hearts[i].vel_y = randomVelocity();
        }
        hearts_data.hearts[i].x += hearts_data.hearts[i].vel_x;
        hearts_data.hearts[i].y += hearts_data.hearts[i].vel_y;
    }
}

void hearts() {
    hearts_data.tick++;

    int hue = (hearts_data.tick / 8) % 360;

    update_hearts();

    set_field_color(strip.hsv2rgb(((hue + 180) % 360), 50, 50));

    render_hearts(hue);

    show_field_ywrapping();
}
