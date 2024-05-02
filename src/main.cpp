#include "common.h"
#include "rainbow_rows.h"
#include "rainbow_solid.h"
#include "rolling_ball.h"

struct Sample {
    void (*reset)();

    void (*run)();
};

#define SAMPLES_SIZE 3
Sample samples[SAMPLES_SIZE] = {
        {&setup_rainbow_rows,  &rainbow_rows},
        {&setup_rainbow_solid, &rainbow_solid},
        {&setup_rolling_ball,  &rolling_ball},
};

Freenove_ESP32_WS2812 strip = Freenove_ESP32_WS2812(LEDS_COUNT, LEDS_PIN, CHANNEL);


void runSample();

void resetAutoLoopTimer();

void chooseSample(ulong tickStart);

void setCurrentSample(int i);

int currentSample;
bool autoLoop;
ulong nextSampleTimer;

void setup() {
    if (FIXED_SAMPLE != -1) {
        currentSample = FIXED_SAMPLE;
        autoLoop = false;
    } else {
        currentSample = 0;
        autoLoop = true;
        resetAutoLoopTimer();
    }
    strip.setBrightness(20);
    strip.begin();
}

void loop() {
    ulong tickStart = millis();
    chooseSample(tickStart);
    runSample();
    strip.show();
    ulong nextTick = tickStart + (1000 / FPS);
    ulong now = millis();
    if (nextTick > now) {
        delay(nextTick - now);
    }
}

void chooseSample(ulong tickStart) {
    if (autoLoop) {
        if (nextSampleTimer < tickStart) {
            resetAutoLoopTimer();
            setCurrentSample(currentSample + 1);
        }
    }
}

void setCurrentSample(int i) {
    currentSample = i % SAMPLES_SIZE;
    samples[currentSample].reset();
}

void runSample() {
    samples[currentSample].run();
}

void resetAutoLoopTimer() {
    nextSampleTimer = millis() + SAMPLE_LENGTH_MS;
}

