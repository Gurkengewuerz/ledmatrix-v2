#include <ACS712.h>
#include <AnimationGradient.h>
#include <AnimationMatrix.h>
#include <AnimationScanLine.h>
#include <AnimationSparkle.h>
#include <AnimationStatic.h>
#include <AnimationStrobe.h>
#include <Arduino.h>
#include <ArduinoJson.h>
#include <Clock.h>
#include <EEPROM.h>
#include <WS281X.h>
#include <WebServer.h>
#include <WebSocketsServer.h>
#include <WiFi.h>
#include <WiFiManager.h>
#include <WiFiUdp.h>
#include <Wire.h>

#include "gitHeader.h"

#define USB_SERIAL Serial
#define SEETINGS_VERSION 0x70

WebServer server(80);
WebSocketsServer webSocket = WebSocketsServer(81);
ACS712 currentSensor(2, 3.3, 4095, 185);  // 0.100 for 20A version. 0.185 for 5A and 0.066 for 30A. Value in Volts per Ampere
WS281X display;
Clock timer(60.0, false);
Clock sensorTimer(0.2, false);
Animation* currentAnimation = nullptr;

const size_t MAX_ANIMATIONS = 25;
Animation** animations = new Animation*[MAX_ANIMATIONS];

struct SettingsStruct {
    uint16_t version;
    char acitveAnimation[33];
    float clockTPS;
    uint8_t brightness;
    uint8_t maxBrightness;
    uint32_t staticColor;
};

const char* settingsPath = "/settings.bin";

void webSocketEvent(uint8_t num, WStype_t type, uint8_t* payload, size_t length) {
    switch (type) {
        case WStype_DISCONNECTED:
            USB_SERIAL.printf("[%u] Disconnected!\n", num);
            break;
        case WStype_CONNECTED: {
            IPAddress ip = webSocket.remoteIP(num);
            USB_SERIAL.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);
        } break;
        case WStype_TEXT: {
            // USB_SERIAL.printf("[%u] get Text\n", num);

            DynamicJsonDocument request(1024);
            DeserializationError error = deserializeJson(request, payload);
            if (error) {
                USB_SERIAL.printf("[%u] deserializeJson() failed: %s\n", num, error.f_str());
                break;
            }

            DynamicJsonDocument response(1024);
            String type = request["event"];
            JsonObject requestData = request["data"];
            if (type.equals("ping")) {
                break;
            } else if (type.equals("display")) {
                response["event"] = type;

                if (requestData.containsKey("brightness")) {
                    uint8_t brightness = requestData["brightness"];
                    display.setBrightness(brightness);
                }

                if (requestData.containsKey("maxBrightness")) {
                    uint8_t maxBrightness = requestData["maxBrightness"];
                    display.setMaxBrightness(maxBrightness);
                }

                if (requestData.containsKey("color")) {
                    JsonObject newColor = requestData["color"];
                    if (newColor.containsKey("r") && newColor.containsKey("g") && newColor.containsKey("b")) {
                        display.setStaticColor(display.getColor(newColor["r"], newColor["g"], newColor["b"]));
                    }
                }

                JsonObject responseData = response.createNestedObject("data");
                responseData["rows"] = display.getRows();
                responseData["cols"] = display.getCols();
                responseData["brightness"] = display.getBrightness();
                responseData["maxBrightness"] = display.getMaxBrightness();

                JsonObject color = response.createNestedObject("color");
                color["r"] = (display.getStaticColor() >> 16) & 0xFF;
                color["g"] = (display.getStaticColor() >> 8) & 0xFF;;
                color["b"] = display.getStaticColor() & 0xFF;
            } else if (type.equals("clock")) {
                response["event"] = type;

                if (requestData.containsKey("tps")) {
                    float tps = requestData["tps"];
                    timer.setTPS(tps);
                }

                if (requestData.containsKey("paused")) {
                    bool paused = requestData["paused"];
                    if (paused)
                        timer.stop();
                    else
                        timer.start();
                }

                JsonObject responseData = response.createNestedObject("data");
                responseData["tps"] = timer.getTPS();
                responseData["paused"] = timer.isPaused();
            } else if (type.equals("animations")) {
                response["event"] = type;

                JsonArray data = response.createNestedArray("data");
                for (size_t i = 0; i < MAX_ANIMATIONS; i++) {
                    if (animations[i] == nullptr) continue;
                    data.add(animations[i]->getName());
                }
            } else if (type.equals("animation")) {
                response["event"] = type;

                if (requestData.containsKey("name")) {
                    bool found = false;
                    for (size_t i = 0; i < MAX_ANIMATIONS; i++) {
                        if (animations[i] == nullptr) continue;
                        if (strcmp(requestData["name"], animations[i]->getName()) != 0) continue;
                        currentAnimation = animations[i];
                        currentAnimation->reset();
                        found = true;
                        break;
                    }
                    if (!found) {
                        currentAnimation = nullptr;
                        display.clear(true);
                    }
                }

                JsonObject responseData = response.createNestedObject("data");
                responseData["name"] = currentAnimation == nullptr ? nullptr : currentAnimation->getName();
            } else if (type.equals("saveSettings")) {
                response["event"] = type;

                SettingsStruct settings = {};
                settings.version = SEETINGS_VERSION;
                settings.brightness = display.getBrightness();
                settings.maxBrightness = display.getMaxBrightness();
                settings.clockTPS = timer.getTPS();
                settings.staticColor = display.getStaticColor();
                strcpy(settings.acitveAnimation, currentAnimation == nullptr ? "" : currentAnimation->getName());

                USB_SERIAL.printf("SAVE SETTINGS struct size: %i\n", sizeof(settings));

                EEPROM.writeBytes(0, (byte*)&settings, sizeof(settings));
                response["data"] = true;
                EEPROM.commit();
            } else {
                // ignore unknown events
                break;
            }

            char json_string[1024];
            serializeJson(response, json_string);
            USB_SERIAL.println(json_string);
            webSocket.broadcastTXT(json_string);
        } break;
        case WStype_BIN:
            USB_SERIAL.printf("[%u] get binary length: %u\n", num, length);
            break;
        case WStype_ERROR:
        case WStype_FRAGMENT_TEXT_START:
        case WStype_FRAGMENT_BIN_START:
        case WStype_FRAGMENT:
        case WStype_FRAGMENT_FIN:
            break;
    }
}

void handle_OnConnect() {
    server.sendHeader("Location", String("http://led.mc8051.de/?ip=" + WiFi.localIP().toString() + "%3A81"), true); 
    server.send(302, "text/plain", "");
}

void handle_NotFound() {
    server.send(404, "text/plain", "404 Not found");
}

void setup() {
    USB_SERIAL.begin(115200);
    EEPROM.begin(1024);
    Wire.begin(5, 4);
    WiFi.setSleep(false);

    char debugBuffer[512];
    sprintf(debugBuffer, "Compiled with c++ version %s\r\n# Version %s %s at %s", __VERSION__, GIT_COMMIT, __DATE__, __TIME__);
    USB_SERIAL.println(debugBuffer);

    WiFiManager wifiManager;
    WiFiManagerParameter config_led_rows("led_rows", "Pixel Rows", "25", 3);
    wifiManager.addParameter(&config_led_rows);
    WiFiManagerParameter config_led_cols("led_cols", "Pixel Columns", "25", 3);
    wifiManager.addParameter(&config_led_cols);
    wifiManager.autoConnect();

    USB_SERIAL.println("Ready");
    USB_SERIAL.print("IP address: ");
    USB_SERIAL.println(WiFi.localIP());

    USB_SERIAL.println("Starting HTTP Server...");
    server.on("/", handle_OnConnect);
    server.onNotFound(handle_NotFound);
    server.begin();

    USB_SERIAL.println("Starting WebSocket...");
    webSocket.begin();
    webSocket.onEvent(webSocketEvent);

    USB_SERIAL.println("Initializing Display...");
    int cols = String(config_led_cols.getValue()).toInt();
    int rows = String(config_led_rows.getValue()).toInt();
    USB_SERIAL.printf("cols: %d rows: %d...\n", cols, rows);
    display.init(cols, rows, 0);

    USB_SERIAL.println("Initializing current sensor...");
    currentSensor.autoMidPoint();

    USB_SERIAL.println("Start animations...");
    for (size_t i = 0; i < MAX_ANIMATIONS; i++) {
        animations[i] = nullptr;
    }
    USB_SERIAL.println("Setting animations...");
    animations[0] = new AnimationStatic();
    animations[1] = new AnimationGradient();
    animations[2] = new AnimationMatrix();
    animations[3] = new AnimationScanLine();
    animations[4] = new AnimationSparkle();
    animations[5] = new AnimationStrobe();

    for (size_t i = 0; i < MAX_ANIMATIONS; i++) {
        if (animations[i] == nullptr) continue;
        animations[i]->init(&display);
    }

    currentAnimation = animations[0];

    USB_SERIAL.println("Load Settings...");
    SettingsStruct settings = {};
    EEPROM.readBytes(0, (byte*)&settings, sizeof(settings));
    if (settings.version == SEETINGS_VERSION) {
        USB_SERIAL.println("Valid Settings found.");

        // valid settings
        display.setMaxBrightness(settings.maxBrightness);
        display.setBrightness(settings.brightness);
        display.setStaticColor(settings.staticColor);
        timer.setTPS(settings.clockTPS);

        for (size_t i = 0; i < MAX_ANIMATIONS; i++) {
            if (animations[i] == nullptr) continue;
            if (strcmp(settings.acitveAnimation, animations[i]->getName()) != 0) continue;
            currentAnimation = animations[i];
            break;
        }
    } else
        USB_SERIAL.println("No Valid Settings found. Skipping...");
}

void loop() {
    webSocket.loop();
    server.handleClient();

    if (timer.update()) {
        if (currentAnimation != nullptr) {
            currentAnimation->update();
        }
    }
    /*if (sensorTimer.update()) {
        USB_SERIAL.println("Sensor update...");

        DynamicJsonDocument sensorResponse(1024);
        sensorResponse["event"] = "sensors";
        JsonObject responseData = sensorResponse.createNestedObject("data");
        responseData["current"] = currentSensor.mA_DC();
        char json_string[1024];
        serializeJson(sensorResponse, json_string);
        webSocket.broadcastTXT(json_string);
    }*/
}