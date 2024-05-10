#include "common.h"
#include "controller.h"

#ifdef START_SERVER
#include "server.h"
#endif

void setup() {
    Serial.begin(115200);
    randomSeed(0);
    setup_controller();
#ifdef START_SERVER
    setup_server();
#endif
}

void loop() {
    loop_controller();
#ifdef START_SERVER
        loop_server();
#endif
}
