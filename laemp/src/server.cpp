#include "common.h"

#ifdef START_SERVER
#include "controller.h"
#include <WiFi.h>
#include "ESPAsyncWebServer.h"

char ssid[] = "ablwlan";
char key[] = "theturtlemoves";
IPAddress ip(192, 168, 0, 103);
IPAddress gateway(192, 168, 0, 1);
IPAddress subnet(255, 255, 255, 0);

char fallBackSsid[] = "laemp";
char fallBackKey[] = "laemplaemp";

AsyncWebServer server(80);
bool fallback_active = false;

void ensure_connected() {
    if (!fallback_active && WiFi.status() != WL_CONNECTED) {
        show_waiting();
        WiFi.config(ip, gateway, subnet);
        WiFi.begin(ssid, key);
        show_waiting();
        delay(1000);
        while (WiFi.status() == WL_IDLE_STATUS) {
            delay(100);
        }

        // Serial.println(WiFi.status());
        if (WiFi.status() == WL_CONNECTED) {
            show_wifi();
        } else {
            //fallback to open our own ap
            show_ap();
            WiFi.softAPConfig(ip, gateway, subnet);
            WiFi.softAP(fallBackSsid, fallBackKey);
            fallback_active = true;
        }
        delay(1000);
    }
}

void loop_server() {
    //do we really want to do this?
    ensure_connected();
}


void onNotFound(AsyncWebServerRequest *request) {
    //Handle Unknown Request
    request->send(404);
}


std::string get_samples_string(std::string **samples, int samplesCount) {
    std::string samples_string = "";
    for (int i = 0; i < samplesCount; i++) {
        samples_string += "\"" + *samples[i] + "\",";
    }
    return samples_string.substr(0, samples_string.length() - 1);
}

void setup_server() {
    ensure_connected();

    server.on("/state", HTTP_GET, [](AsyncWebServerRequest *request) {
        State state = get_current_state();
        char message[1024];
        snprintf(message, 1024,
                 R"({"currentSample":"%d", "autoloop":"%s", "random":"%s", "samples":[%s]})",
                 state.currentSample,
                 state.autoloop ? "true" : "false",
                 state.random ? "true" : "false",
                 get_samples_string(state.samples, state.samplesCount).c_str());
        request->send(200, "application/json", message);
        free(state.samples);
    });

    server.on("/sample", HTTP_GET, [](AsyncWebServerRequest *request) {
        const String number = request->getParam("number")->value();
        try {
            int sample = std::stoi(number.c_str());
            set_wanted_sample(sample);
            request->send(200, "text/plain", "set");
        } catch (...) {
            request->send(400, "text/plain", "invalid number: " + number);
        }
    });

    server.on("/autoloop", HTTP_GET, [](AsyncWebServerRequest *request) {
        const String value = request->getParam("on")->value();
        set_autoloop(value.equals("true"));
        request->send(200, "text/plain", "set");
    });

    server.on("/random", HTTP_GET, [](AsyncWebServerRequest *request) {
        const String value = request->getParam("on")->value();
        set_random(value.equals("true"));
        request->send(200, "text/plain", "set");
    });

    server.onNotFound(onNotFound);

    server.begin();
}

#endif
