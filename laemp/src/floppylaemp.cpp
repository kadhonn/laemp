#include "field.h"
#include "figures.h"

#define NUM_PIPES 3
#define MIN_PIPE_POS (FIELD_HEIGHT/100.0*30)
#define MAX_PIPE_POS (FIELD_HEIGHT/100.0*70)
#define PIPE_POS_SIZE (FIELD_HEIGHT/100.0*40)
#define PIPE_MOVE_SPEED (0.2)
#define PIPE_MOVE_DELAY (1.5*FPS)
#define JUMPER_X_SPEED (0.3)
#define JUMPER_Y_SPEED (100.0)
#define JUMPER_MAX_WOBBLE (PIPE_POS_SIZE / 3.0)
#define JUMPER_WOBBLE_SPEED (4.0 / FPS)

struct Pipe {
    int x;
    float wantedPos;
    float currentPos;
    int delay;
};

struct FloppyLaempData {
    int tick;
    float jumperX;
    float jumperY;
    float jumberYV;
    int lastNextPipe;
    Pipe pipes[NUM_PIPES];
};

FloppyLaempData floppy_laemp_data;

float randomPipePos() {
    return random(MIN_PIPE_POS, MAX_PIPE_POS);
}

void setup_floppylaemp() {
    floppy_laemp_data.tick = 0;
    floppy_laemp_data.jumperX = 0;
    floppy_laemp_data.jumperY = FIELD_HEIGHT / 2;
    floppy_laemp_data.jumberYV = 0;
    for (int i = 0; i < NUM_PIPES; i++) {
        float startPos = randomPipePos();
        floppy_laemp_data.pipes[i] = Pipe{
            FIELD_WIDTH / NUM_PIPES * i,
            startPos,
            startPos
        };
    }
}

void render_floppylaemp() {
    set_field_color(0);

    uint32_t huePipe = (floppy_laemp_data.tick / 8) % 360;
    uint32_t colorPipe = strip.hsv2rgb(huePipe, 80, 100);
    for (int i = 0; i < NUM_PIPES; i++) {
        Pipe &currentPipe = floppy_laemp_data.pipes[i];
        int x_pos = currentPipe.x;

        for (int i = 0; i < currentPipe.currentPos - PIPE_POS_SIZE / 2; i += 1) {
            paint_figure(x_pos, i, 1, 1, colorPipe, figure_bubble);
        }
        for (int i = currentPipe.currentPos + PIPE_POS_SIZE / 2; i < FIELD_HEIGHT; i += 1) {
            paint_figure(x_pos, i, 1, 1, colorPipe, figure_bubble);
        }
    }

    uint32_t hueJumper = (floppy_laemp_data.tick / 8 + 180) % 360;
    uint32_t colorJumper = strip.hsv2rgb(hueJumper, 80, 100);
    float yWobble = JUMPER_MAX_WOBBLE * abs(sin(floppy_laemp_data.tick * JUMPER_WOBBLE_SPEED)) - JUMPER_MAX_WOBBLE / 2;
    paint_figure(floppy_laemp_data.jumperX, floppy_laemp_data.jumperY + yWobble, 1, 1, colorJumper, figure_bubble);
}

int get_next_pipe() {
    int i = 0;
    while (i < NUM_PIPES && floppy_laemp_data.jumperX > floppy_laemp_data.pipes[i].x) {
        i++;
    }
    return i % NUM_PIPES;
}

void update_floppylaemp() {
    int nextPipe = get_next_pipe();
    if (nextPipe != floppy_laemp_data.lastNextPipe) {
        floppy_laemp_data.pipes[floppy_laemp_data.lastNextPipe].wantedPos = randomPipePos();
        floppy_laemp_data.pipes[floppy_laemp_data.lastNextPipe].delay = PIPE_MOVE_DELAY;
        floppy_laemp_data.lastNextPipe = nextPipe;
    }

    floppy_laemp_data.jumperX += JUMPER_X_SPEED;
    if (floppy_laemp_data.jumperX >= FIELD_WIDTH) {
        floppy_laemp_data.jumperX -= FIELD_WIDTH;
    }
    floppy_laemp_data.jumperY += (floppy_laemp_data.pipes[nextPipe].wantedPos - floppy_laemp_data.jumperY) / JUMPER_Y_SPEED;

    for (int i = 0; i < NUM_PIPES; i++) {
        Pipe &currentPipe = floppy_laemp_data.pipes[i];
        if (currentPipe.currentPos != currentPipe.wantedPos) {
            if (floppy_laemp_data.pipes[floppy_laemp_data.lastNextPipe].delay > 0) {
                floppy_laemp_data.pipes[floppy_laemp_data.lastNextPipe].delay--;
            } else {
                if (abs(currentPipe.currentPos - currentPipe.wantedPos) < PIPE_MOVE_SPEED) {
                    currentPipe.currentPos = currentPipe.wantedPos;
                } else {
                    if (currentPipe.currentPos > currentPipe.wantedPos) {
                        currentPipe.currentPos -= PIPE_MOVE_SPEED;
                    } else {
                        currentPipe.currentPos += PIPE_MOVE_SPEED;
                    }
                }
            }
        }
    }
}

void floppylaemp() {
    floppy_laemp_data.tick++;

    update_floppylaemp();

    render_floppylaemp();

    show_field();
}
