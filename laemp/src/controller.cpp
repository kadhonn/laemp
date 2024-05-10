#include "common.h"
#include "controller.h"
#include "field.h"
#include "rainbow_rows.h"
#include "rainbow_solid.h"
#include "rolling_ball.h"
#include "vertical_stripes.h"
#include "hearts.h"
#include "bubbles.h"
#include "noise_sample.h"

struct Sample {
    std::string name;

    void (*reset)();

    void (*run)();
};

#define FIXED_SAMPLE (-1)
#define SAMPLES_SIZE 7
Sample samples[SAMPLES_SIZE] = {
        {"Rainbow Rows",    &setup_rainbow_rows,     &rainbow_rows},
        {"Rainbow Solid",   &setup_rainbow_solid,    &rainbow_solid},
        {"Rolling Ball",    &setup_rolling_ball,     &rolling_ball},
        {"Rainbow Columns", &setup_vertical_stripes, &vertical_stripes},
        {"Hearts",          &setup_hearts,           &hearts},
        {"Bubbles",         &setup_bubbles,          &bubbles},
        {"Noise",           &setup_noise,            &noise},
};

Freenove_ESP32_WS2812 strip = Freenove_ESP32_WS2812(LEDS_COUNT, LEDS_PIN, CHANNEL);

int currentSample;
int wantedSample;
bool autoLoop;
bool randomLoop;
ulong nextSampleTimer;

void set_wanted_sample(int i) {
    wantedSample = i;
}

void set_current_sample(int i) {
    currentSample = ((i % SAMPLES_SIZE) + SAMPLES_SIZE) % SAMPLES_SIZE;
    samples[currentSample].reset();
}

void runSample() {
    samples[currentSample].run();
}

void resetAutoLoopTimer() {
    nextSampleTimer = millis() + SAMPLE_LENGTH_MS;
}

void chooseSample(ulong tickStart) {
    if (wantedSample != -1) {
        set_current_sample(wantedSample);
        wantedSample = -1;
    } else {
        if (autoLoop) {
            if (nextSampleTimer < tickStart) {
                resetAutoLoopTimer();
                if (randomLoop) {
                    set_current_sample(random(0, SAMPLES_SIZE));
                } else {
                    set_current_sample(currentSample + 1);
                }
            }
        }
    }
}

void setup_controller() {
    wantedSample = -1;
    set_random(false);
    if (FIXED_SAMPLE != -1) {
        set_current_sample(FIXED_SAMPLE);
        set_autoloop(false);
    } else {
        set_current_sample(0);
        set_autoloop(true);
    }
    setup_field();
    strip.setBrightness(60);
//    strip.setBrightness(90); //is max
    strip.begin();
}

void loop_controller() {
    ulong tickStart = millis();
    chooseSample(tickStart);
    runSample();
    strip.show();
    ulong nextTick = tickStart + (1000 / FPS);
    ulong now = millis();
    if (nextTick > now) {
        delay(nextTick - now);
    } else {
        //for performance debugging
//        Serial.print("!");
//        Serial.println((long)nextTick - (long)now);
    }
}

//user has to free state object sample_names array
State get_current_state() {
    std::string **sample_names = static_cast<std::string **>(malloc(SAMPLES_SIZE * sizeof(std::string)));
    for (int i = 0; i < SAMPLES_SIZE; i++) {
        sample_names[i] = &samples[i].name;
    }
    return State{
            currentSample,
            autoLoop,
            false,
            sample_names,
            SAMPLES_SIZE
    };
}

void set_autoloop(bool autoloop_new) {
    autoLoop = autoloop_new;
    resetAutoLoopTimer();
}

void set_random(bool random) {
    randomLoop = random;
}