#include <Arduino.h>
#include <WiFi.h>
#include <ArduinoJson.h>
#include <PubSubClient.h>

// Comment this before uploading to the actual device
#define TESTDEBUG
#define TOPIC "esp32/lampStates"

// Replace the next variables with your SSID/Password combination
const char *ssid = "ssid_name";
const char *password = "ssid_password";

// Add your MQTT Broker IP address, example:
// const char* mqtt_server = "192.168.1.144";
const char *mqtt_server = "http://broker.hivemq.com/";

WiFiClient espClient;
PubSubClient client(espClient);
// long lastMsg = 0;
// char msg[50];
// int value = 0;

const uint8_t ledPin[8] = {18, 5, 17, 16, 4, 0, 2, 15};
const uint8_t ledChannel[8] = {0, 1, 2, 3, 4, 5, 6, 7}; // Max 16 Channel

enum
{
  LampState_Off,
  LampState_Dim = 50,
  LampState_Bright = 100
};

uint8_t lampState[8] = {0};

// setting PWM properties
const int freq = 5000;
const int resolution = 8;

void setDutyCycleAtLamp(uint8_t dutyCycle, uint8_t lampNo);
void setup_wifi();
void callback(char *topic, byte *message, unsigned int length);
void reconnect();

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(115200);

#ifndef TESTDEBUG
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
#endif

  // PWM LED PIN Initialization
  for (uint8_t i = 0; i < 8; i++)
  {
    // configure LED PWM functionalitites
    ledcSetup(ledChannel[i], freq, resolution);
    // attach the channel to the GPIO to be controlled
    ledcAttachPin(ledPin[i], ledChannel[i]);
  }
}

void loop()
{
  // put your main code here, to run repeatedly:
#ifndef TESTDEBUG
  if (!client.connected())
  {
    reconnect();
  }
  client.loop();
#else
  String msgStr = "{\"led1\":100, \"led2\":0, \"led3\":0, \"led4\":50, \"led5\":0, \"led6\":0, \"led7\":0, \"led8\":100}";
  // Serial.println(String() + "msgStr: " + msgStr.length());
  const uint8_t length = msgStr.length();
  byte msgByte[length + 1];
  char topic[50];
  strcpy(topic, TOPIC);

  for (uint8_t i = 0; i < msgStr.length(); i++)
  {
    msgByte[i] = (byte)msgStr.charAt(i);
  }
  callback(topic, msgByte, msgStr.length());
#endif

  for (uint8_t i = 0; i < 8; i++)
  {
    setDutyCycleAtLamp(lampState[i], i);
  }

  delay(1000);
}

void setDutyCycleAtLamp(uint8_t dutyCycle, uint8_t lampNo)
{
  // Serial.println(String() + "Set Lamp " + lampNo + " " + dutyCycle + "%");
  dutyCycle = constrain(dutyCycle, 0, 100);
  dutyCycle = map(dutyCycle, 0, 100, 0, 255);
  lampNo = constrain(lampNo, 0, 8);
  ledcWrite(ledChannel[lampNo], dutyCycle);
}

void setup_wifi()
{
  delay(10);
  // We start by connecting to a WiFi network
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
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

/// JSON data format of the message
/// {"led1":0, "led2":0, "led3":0, "led4":0, "led5":0, "led6":0, "led7":0, "led8":0}
void callback(char *topic, byte *message, unsigned int length)
{
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageTemp = "";

  for (int i = 0; i < length; i++)
  {
    messageTemp += (char)message[i];
  }
  Serial.print(messageTemp);
  Serial.println();

  // Feel free to add more if statements to control more GPIOs with MQTT

  // If a message is received on the topic esp32/output, you check if the message is either "on" or "off".
  // Changes the output state according to the message
  if (String(topic) == TOPIC)
  {
    Serial.println("Topic is matched!");

    JsonDocument doc;
    deserializeJson(doc, messageTemp);
    JsonObject obj = doc.as<JsonObject>();
    for (uint8_t i = 0; i < 8; i++)
    {
      lampState[i] = (uint8_t)obj[String("led") + (i + 1)];
    }
  }
}

void reconnect()
{
  // Loop until we're reconnected
  while (!client.connected())
  {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP32Client"))
    {
      Serial.println("connected");
      // Subscribe
      client.subscribe(TOPIC);
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