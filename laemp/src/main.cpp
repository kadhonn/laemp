#include "common.h"
#include "field.h"
#include "rainbow_rows.h"
#include "rainbow_solid.h"
#include "rolling_ball.h"
#include "vertical_stripes.h"
#include "hearts.h"

struct Sample {
    void (*reset)();

    void (*run)();
};

#define FIXED_SAMPLE (4)
#define SAMPLES_SIZE 5
Sample samples[SAMPLES_SIZE] = {
        {&setup_rainbow_rows,  &rainbow_rows},
        {&setup_rainbow_solid, &rainbow_solid},
        {&setup_rolling_ball,  &rolling_ball},
        {&setup_vertical_stripes,  &vertical_stripes},
        {&setup_hearts,  &hearts},
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
    Serial.begin(115200);
    if (FIXED_SAMPLE != -1) {
        currentSample = FIXED_SAMPLE;
        autoLoop = false;
    } else {
        currentSample = 0;
        autoLoop = true;
        resetAutoLoopTimer();
    }
    setup_field();
    strip.setBrightness(60);
//    strip.setBrightness(90); //is max
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

