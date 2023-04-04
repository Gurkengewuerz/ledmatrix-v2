#include <ACS712.h>
#include <AnimationGradient.h>
#include <AnimationMatrix.h>
#include <AnimationScanLine.h>
#include <AnimationSparkle.h>
#include <AnimationStrobe.h>
#include <Arduino.h>
#include <ArduinoJson.h>
#include <Clock.h>
#include <ESPmDNS.h>
#include <WS281X.h>
#include <WebSocketsServer.h>
#include <WiFi.h>
#include <WiFiManager.h>
#include <WiFiUdp.h>
#include <Wire.h>

#define USB_SERIAL Serial

WebSocketsServer webSocket = WebSocketsServer(81);
ACS712 currentSensor(2, 3.3, 4095, 185);  // 0.100 for 20A version. 0.185 for 5A and 0.066 for 30A. Value in Volts per Ampere
WS281X display;
Clock timer(60.0, false);
Clock sensorTimer(0.2, false);
Animation* currentAnimation = nullptr;

const size_t MAX_ANIMATIONS = 25;
Animation** animations = new Animation*[MAX_ANIMATIONS];

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
            USB_SERIAL.printf("[%u] get Text\n", num);

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

                JsonObject responseData = response.createNestedObject("data");
                responseData["rows"] = display.getRows();
                responseData["cols"] = display.getCols();
                responseData["brightness"] = display.getBrightness();
                responseData["maxBrightness"] = display.getMaxBrightness();
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
            } else {
                response["event"] = "unknown";
            }
            USB_SERIAL.println("test");
            char json_string[1024];
            serializeJson(response, json_string);
            USB_SERIAL.println(json_string);
            webSocket.sendTXT(num, json_string);
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

void setup() {
    USB_SERIAL.begin(115200);

    Wire.begin(5, 4);

    WiFiManager wifiManager;
    WiFiManagerParameter config_led_rows("led_rows", "Pixel Rows", "25", 3);
    wifiManager.addParameter(&config_led_rows);
    WiFiManagerParameter config_led_cols("led_cols", "Pixel Columns", "25", 3);
    wifiManager.addParameter(&config_led_cols);
    wifiManager.autoConnect();

    USB_SERIAL.println("Ready");
    USB_SERIAL.print("IP address: ");
    USB_SERIAL.println(WiFi.localIP());

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
    animations[0] = new AnimationGradient();
    animations[1] = new AnimationMatrix();
    animations[2] = new AnimationScanLine();
    animations[3] = new AnimationSparkle();
    animations[4] = new AnimationStrobe();

    for (size_t i = 0; i < MAX_ANIMATIONS; i++) {
        if (animations[i] == nullptr) continue;
        animations[i]->init(&display);
    }
}

void loop() {
    webSocket.loop();

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