#include "common.h"

#pragma once

struct State {
    int currentSample;
    bool loop;
    bool random;
    std::string **samples;
    int samplesCount;
};

State get_current_state();

void set_wanted_sample(int i);

void set_autoloop(bool autoloop);

void set_random(bool random);

void setup_controller();

void loop_controller();
