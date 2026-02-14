#include "field.h"
#include "figures.h"

#define NUM_PAINTER 2

struct Painter {
    float x;
    float y;
    float vel_x;
    float vel_y;
    int change_vel;
};

struct PainterData {
    int tick;
    Painter painter[NUM_PAINTER];
};

PainterData painter_data;


float random_painter_velocity() {
    return ((float) random(0, 100) - 50.f) / 150.f;
}

int random_painter_duration() {
    return random(1 * FPS, 3 * FPS);
}

void setup_painter() {
    painter_data.tick = 0;
    for (int i = 0; i < NUM_PAINTER; i++) {
        painter_data.painter[i] = Painter{
            (float) random(0, FIELD_WIDTH),
            (float) random(0, FIELD_HEIGHT),
            random_painter_velocity(),
            random_painter_velocity(),
            random_painter_duration(),
        };
    }
}

void render_painter() {
    for (int i = 0; i < FIELD_HEIGHT; i++) {
        for (int j = 0; j < FIELD_WIDTH; j++) {
            uint32_t color = get_field_value(j, i);
            uint32_t r = (color >> 16 & 0xFF);
            uint32_t g = (color >> 8 & 0xFF);
            uint32_t b = (color & 0xFF);

            if (r > 0) {
                r -= 1;
            }
            if (g > 0) {
                g -= 1;
            }
            if (b > 0) {
                b -= 1;
            }

            set_field_value(r << 16 | g << 8 | b, j, i);
        }
    }
    for (int i = 0; i < NUM_PAINTER; i++) {
        Painter currentPainter = painter_data.painter[i];
        int x_pos = (int) round(currentPainter.x);
        int y_pos = (int) round(currentPainter.y);
        uint32_t hue = (painter_data.tick / 8 + 360 / NUM_PAINTER * i) % 360;
        uint32_t color = strip.hsv2rgb(hue, 100, 100);

        paint_figure_ywrapping(x_pos, y_pos, 3, 3, color, figure_bubble);
    }
}

void update_painter() {
    for (int i = 0; i < NUM_PAINTER; i++) {
        Painter &currentPainter = painter_data.painter[i];
        if (currentPainter.change_vel <= painter_data.tick) {
            currentPainter.change_vel = painter_data.tick + random_painter_duration();
            currentPainter.vel_x = random_painter_velocity();
            currentPainter.vel_y = random_painter_velocity();
        }
        currentPainter.x += currentPainter.vel_x;
        currentPainter.y += currentPainter.vel_y;
    }
}

void painter() {
    painter_data.tick++;

    update_painter();

    render_painter();

    show_field_ywrapping();
}
