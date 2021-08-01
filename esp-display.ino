#include <ESP8266WiFi.h>
#include <PubSubClient.h>

const char *ssid = "no place like 127.0.0.1 - iot";
const char *password = "***";

String _displayTime = "";
String _displayTempIn = "";
String _displayTempInLabel = "";
String _displayTempOut = "";
String _displayTempOutLabel = "";

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

void setup()
{
    Serial.begin(115200);

    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);

    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());

    client.setServer("192.168.5.212", 1883);
    client.setCallback(callback);

    // TODO init display
}

void loop()
{
    if (!client.connected())
    {
        reconnect();
    }
    client.loop();
}

void callback(char *topic, byte *payload, unsigned int length)
{
    Serial.print("Message arrived [");
    Serial.print(topic);
    Serial.println("] ");

    if (strcmp(topic, "espDisplay/time") == 0)
    {
        _displayTime = "";
        for (int i = 0; i < length; i++)
        {
            _displayTime += (char)payload[i];
        }
    }

    if (strcmp(topic, "espDisplay/tempIn") == 0)
    {
        _displayTempIn = "";
        for (int i = 0; i < length; i++)
        {
            _displayTempIn += (char)payload[i];
        }
    }

    if (strcmp(topic, "espDisplay/tempInLabel") == 0)
    {
        _displayTempInLabel = "";
        for (int i = 0; i < length; i++)
        {
            _displayTempInLabel += (char)payload[i];
        }
    }

    if (strcmp(topic, "espDisplay/tempOut") == 0)
    {
        _displayTempOut = "";
        for (int i = 0; i < length; i++)
        {
            _displayTempOut += (char)payload[i];
        }
    }

    if (strcmp(topic, "espDisplay/tempOutLabel") == 0)
    {
        _displayTempOutLabel = "";
        for (int i = 0; i < length; i++)
        {
            _displayTempOutLabel += (char)payload[i];
        }
    }

    // TODO update display/display text
}

void reconnect()
{
    // Loop until we're reconnected
    while (!client.connected())
    {
        Serial.print("Attempting MQTT connection...");
        // Create a random client ID
        String clientId = "ESP8266Client-";
        clientId += String(random(0xffff), HEX);
        // Attempt to connect
        if (client.connect(clientId.c_str()))
        {
            Serial.println("connected");

            client.subscribe("espDisplay/time");
            client.subscribe("espDisplay/tempIn");
            client.subscribe("espDisplay/tempInLabel");
            client.subscribe("espDisplay/tempOut");
            client.subscribe("espDisplay/tempOutLabel");
        }
        else
        {
            Serial.print("failed, rc=");
            Serial.print(client.state());
            Serial.println(" try again in 5 seconds");
            // Wait 5 seconds before retrying
            delay(5000);
        }
    }
}