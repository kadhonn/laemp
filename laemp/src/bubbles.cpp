#include "figures.h"

#define MAX_BUBBLES 30

struct Bubble {
    bool active;
    float x;
    float y;
    float speed;
    int size;
};

struct BubblesData {
    int tick;
    long next_bubble;
    Bubble bubbles[MAX_BUBBLES];
};

BubblesData bubbles_data;

void setup_bubbles() {
    bubbles_data.tick = 0;
    bubbles_data.next_bubble = 0;
    for (int i = 0; i < MAX_BUBBLES; i++) {
        bubbles_data.bubbles[i] = Bubble{false, 0, 0, 0, 0};
    }
}

int find_empty_index() {
    for (int i = 0; i < MAX_BUBBLES; i++) {
        if (!bubbles_data.bubbles[i].active) {
            return i;
        }
    }
    return -1;
}

void try_spawn_bubble() {
    int empty_index = find_empty_index();
    if (empty_index != -1) {
        bubbles_data.bubbles[empty_index] = Bubble{
                true,
                (float) random(0, FIELD_WIDTH),
                -80,
                ((float) random(50, 250)) / 400,
                random(1,5)
        };
    }
}

void render_bubbles(int hue) {
    uint32_t bubble_color = strip.hsv2rgb(hue, 10, 100);
    int8_t *figure = figure_bubble;

    for (int i = 0; i < MAX_BUBBLES; i++) {
        Bubble bubble = bubbles_data.bubbles[i];
        if (bubble.active) {
            paint_figure(bubble.x, bubble.y, bubble.size, bubble.size, bubble_color, figure);
        }
    }
}

void update_bubble(int i) {
    Bubble *bubble = &bubbles_data.bubbles[i];
    bubble->y += bubble->speed;
    if (bubble->y > FIELD_HEIGHT + 80) {
        bubble->active = false;
    }
}

void update_bubbles() {
    for (int i = 0; i < MAX_BUBBLES; i++) {
        if (bubbles_data.bubbles[i].active) {
            update_bubble(i);
        }
    }
}

void bubbles() {
    if (bubbles_data.next_bubble <= bubbles_data.tick) {
        try_spawn_bubble();
        bubbles_data.next_bubble = bubbles_data.tick + random(10, 40);
    }

    update_bubbles();

    int hue = bubbles_data.tick / 8 % 360;
    set_field_color(strip.hsv2rgb(hue, 100, 50));

    render_bubbles(hue);

    show_field();

    bubbles_data.tick++;
}
