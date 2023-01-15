#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <U8g2lib.h>
#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

U8G2_SSD1309_128X64_NONAME0_F_4W_SW_SPI u8g2(U8G2_R3, /* clock=*/0, /* data=*/4, /* cs=*/14, /* dc=*/12, /* reset=*/13);

const char *ssid = "no place like 127.0.0.1 - iot";
const char *password = "***";

String _displayTime = "";
String _displayTempIn = "";
String _displayTempInHum = "";
String _displayTempInLabel = "";
String _displayTempOut = "";
String _displayTempOutHum = "";
String _displayTempOutLabel = "";
int _displayState = 0;

int button1Pin = 16;
int _button1State = 0;
int button2Pin = 15;
int _button2State = 0;

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

void setup() {
    Serial.begin(115200);

    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);

    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());

    client.setServer("192.168.5.212", 1883);
    client.setCallback(callback);

    u8g2.begin();
    u8g2.setFlipMode(0);
    u8g2.setFontDirection(0);

    pinMode(button1Pin, INPUT);
    pinMode(button2Pin, INPUT);
}

void loop() {
    if (!client.connected()) {
        reconnect();
    }
    client.loop();

    readButtons();

    delay(100);
}

void readButtons() {
    int tmpState = 0;

    tmpState = digitalRead(button1Pin);
    if (tmpState != _button1State) {
        _button1State = tmpState;
        client.publish("espDisplay/button1Press", _button1State == HIGH ? "1" : "0");
        Serial.println("Button 1 press changed: " + String(_button1State == HIGH ? "pressed" : "released"));
    }

    tmpState = digitalRead(button2Pin);
    if (tmpState != _button2State) {
        _button2State = tmpState;
        client.publish("espDisplay/button2Press", _button2State == HIGH ? "1" : "0");
        Serial.println("Button 2 press changed: " + String(_button2State == HIGH ? "pressed" : "released"));
    }
}

void callback(char *topic, byte *payload, unsigned int length) {
    String message = "";
    for (int i = 0; i < length; i++) {
        message += (char)payload[i];
    }

    Serial.println("Message arrived [" + String(topic) + "]: [" + message + "]");

    if (strcmp(topic, "espDisplay/time") == 0) {
        _displayTime = message;
    }

    if (strcmp(topic, "espDisplay/tempIn") == 0) {
        _displayTempIn = message;
    }

    if (strcmp(topic, "espDisplay/tempInHum") == 0) {
        _displayTempInHum = message + "%";
    }

    if (strcmp(topic, "espDisplay/tempInLabel") == 0) {
        _displayTempInLabel = message;
    }

    if (strcmp(topic, "espDisplay/tempOut") == 0) {
        _displayTempOut = message;
    }

    if (strcmp(topic, "espDisplay/tempOutHum") == 0) {
        _displayTempOutHum = message + "%";
    }

    if (strcmp(topic, "espDisplay/tempOutLabel") == 0) {
        _displayTempOutLabel = message;
    }

    if (strcmp(topic, "espDisplay/displayState") == 0) {
        _displayState = message.equals("on") ? 1 : 0;
    }

    updateDisplay();
}

void reconnect() {
    while (!client.connected()) {
        Serial.print("Attempting MQTT connection...");
        // Create a random client ID
        String clientId = "ESP8266Client-";
        clientId += String(random(0xffff), HEX);
        // Attempt to connect
        if (client.connect(clientId.c_str())) {
            Serial.println("connected");

            client.subscribe("espDisplay/time");
            client.subscribe("espDisplay/tempIn");
            client.subscribe("espDisplay/tempInHum");
            client.subscribe("espDisplay/tempInLabel");
            client.subscribe("espDisplay/tempOut");
            client.subscribe("espDisplay/tempOutHum");
            client.subscribe("espDisplay/tempOutLabel");
            client.subscribe("espDisplay/displayState");
        } else {
            Serial.print("failed, rc=");
            Serial.print(client.state());
            Serial.println(" try again in 5 seconds");
            // Wait 5 seconds before retrying
            delay(5000);
        }
    }
}

void updateDisplay() {
    if (_displayState == 0) {
        u8g2.setPowerSave(1);
        return;
    } else {
        u8g2.setPowerSave(0);
    }

    int x;
    int y;

    u8g2.firstPage();
    do {
        x = 8;
        u8g2.setFont(u8g2_font_t0_11_tf);

        // time
        u8g2.drawStr(17, x, _displayTime.c_str());
        x += 32;

        // binary clock
        y = 14;
        drawBinaryNumber(x, y, (int)_displayTime.charAt(0));
        y += 8;
        drawBinaryNumber(x, y, (int)_displayTime.charAt(1));
        y += 12;
        drawBinaryNumber(x, y, (int)_displayTime.charAt(3));
        y += 8;
        drawBinaryNumber(x, y, (int)_displayTime.charAt(4));

        x += 22;
        // inside
        u8g2.drawStr(0, x + 5, _displayTempInLabel.c_str());
        u8g2.drawStr(64 - _displayTempIn.length() * 6, x, _displayTempIn.c_str());
        x += 9;
        u8g2.drawStr(64 - _displayTempInHum.length() * 6, x, _displayTempInHum.c_str());
        x += 18;

        // outside
        u8g2.drawStr(0, x + 5, _displayTempOutLabel.c_str());
        u8g2.drawStr(64 - _displayTempOut.length() * 6, x, _displayTempOut.c_str());
        x += 9;
        u8g2.drawStr(64 - _displayTempOutHum.length() * 6, x, _displayTempOutHum.c_str());
    } while (u8g2.nextPage());
}

void drawBinaryNumber(int xStart, int y, int number) {
    drawBox(xStart, y, number & 1);
    drawBox(xStart - 8, y, number & 2);
    drawBox(xStart - 16, y, number & 4);
    drawBox(xStart - 24, y, number & 8);
}

void drawBox(int x, int y, bool filled) {
    // CAUTION: the display rotation (line 11 ~> U8G2_R3) seems not to be regarded when drawing boxes/frames...
    if (filled) {
        u8g2.drawBox(y, x, 6, 6);
    } else {
        u8g2.drawFrame(y, x, 6, 6);
    }
}