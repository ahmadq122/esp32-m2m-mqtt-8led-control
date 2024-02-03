#include <Arduino.h>
#include <WiFi.h>
#include <ArduinoJson.h>
#include <PubSubClient.h>

#if CONFIG_FREERTOS_UNICORE
#define ARDUINO_RUNNING_CORE 0
#else
#define ARDUINO_RUNNING_CORE 1
#endif

// Comment this before uploading to the actual device
#define TESTDEBUG

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

/// btnPinNIndex consist of 2 bytes data
/// byte 1 (MSB) is the Pin Number
/// byte 2 (LSB) is the Index number of the PIN
// const uint8_t btnPinNIndex[8] = {18, 5, 17, 16, 4, 0, 2, 15};
const uint16_t btnPinNIndex[8] = {(uint16_t)((18 << 8) | (0)),
                                  (uint16_t)((5 << 8) | (1)),
                                  (uint16_t)((17 << 8) | (2)),
                                  (uint16_t)((16 << 8) | (3)),
                                  (uint16_t)((4 << 8) | (4)),
                                  (uint16_t)((0 << 8) | (5)),
                                  (uint16_t)((2 << 8) | (6)),
                                  (uint16_t)((15 << 8) | (7))};
uint8_t lampState[8] = {0};
enum
{
  LampState_Off,
  LampState_Dim = 50,
  LampState_Bright = 100
};

// Define 8 tasks for reading the button state.
void TaskButton1(void *pvParameters);
void TaskButton2(void *pvParameters);
void TaskButton3(void *pvParameters);
void TaskButton4(void *pvParameters);
void TaskButton5(void *pvParameters);
void TaskButton6(void *pvParameters);
void TaskButton7(void *pvParameters);
void TaskButton8(void *pvParameters);

void setup_wifi();
void reconnect();
void callback(char *topic, byte *message, unsigned int length);

// The setup function runs once when you press reset or power on the board.
void setup()
{
  // Initialize serial communication at 115200 bits per second:
  Serial.begin(115200);

#ifndef TESTDEBUG
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
#endif

  // Set up two tasks to run independently.
  xTaskCreate(
      TaskButton1, "Task Read Button 1" // A name just for humans
      ,
      2048 // The stack size can be checked by calling `uxHighWaterMark = uxTaskGetStackHighWaterMark(NULL);`
      ,
      (void *)&btnPinNIndex[0] // Task parameter which can modify the task behavior. This must be passed as pointer to void. When no parameter is used, simply pass NULL
      ,
      1 // Priority
      ,
      NULL // Task handle is not used here - simply pass NULL
  );
  xTaskCreate(TaskButton2, "Task Read Button 2", 2048, (void *)&btnPinNIndex[1], 2, NULL);
  xTaskCreate(TaskButton3, "Task Read Button 3", 2048, (void *)&btnPinNIndex[2], 3, NULL);
  xTaskCreate(TaskButton4, "Task Read Button 4", 2048, (void *)&btnPinNIndex[3], 4, NULL);
  xTaskCreate(TaskButton5, "Task Read Button 5", 2048, (void *)&btnPinNIndex[4], 5, NULL);
  xTaskCreate(TaskButton6, "Task Read Button 6", 2048, (void *)&btnPinNIndex[5], 6, NULL);
  xTaskCreate(TaskButton7, "Task Read Button 7", 2048, (void *)&btnPinNIndex[6], 7, NULL);
  xTaskCreate(TaskButton8, "Task Read Button 8", 2048, (void *)&btnPinNIndex[7], 8, NULL);
}

void loop()
{
#ifndef TESTDEBUG
  if (!client.connected())
  {
    reconnect();
  }
  client.loop();
#endif

  String message, messageFinal;
  char msgJson[8][13];
  for (uint8_t i = 0; i < 8; i++)
  {
    sprintf(msgJson[i], "\"led%d\":%d", i + 1, lampState[i]);
    message += String(msgJson[i]) + (i < 7 ? ", " : "");
  }
  messageFinal = "{" + message + "}";
  client.publish("esp32/lampStates", messageFinal.c_str());

  Serial.println(messageFinal);
  delay(1000);
}

void readButtonInput(uint8_t btnPin, uint8_t lampNo)
{
  // initialize digital btnPin on pin pvParameters as an input.
  pinMode(btnPin, INPUT_PULLUP);

  while (1)
  {
    if (!digitalRead(btnPin))
    {
      if (lampState[lampNo] == LampState_Off)
        lampState[lampNo] = LampState_Dim;
      else if (lampState[lampNo] == LampState_Dim)
        lampState[lampNo] = LampState_Bright;
      else if (lampState[lampNo] == LampState_Bright)
        lampState[lampNo] = LampState_Off;

      while (!digitalRead(btnPin))
        ;
    }
    delay(100);
  }
}

/*--------------------------------------------------*/
/*---------------------- Tasks ---------------------*/
/*--------------------------------------------------*/

void TaskButton1(void *pvParameters)
{ // This is a task.
  uint8_t btnPin = (uint8_t)(*((uint16_t *)pvParameters) >> 8);
  uint8_t lampNo = (uint8_t) * ((uint16_t *)pvParameters);

  readButtonInput(btnPin, lampNo);
}
void TaskButton2(void *pvParameters)
{ // This is a task.
  uint8_t btnPin = (uint8_t)(*((uint16_t *)pvParameters) >> 8);
  uint8_t lampNo = (uint8_t) * ((uint16_t *)pvParameters);

  readButtonInput(btnPin, lampNo);
}
void TaskButton3(void *pvParameters)
{ // This is a task.
  uint8_t btnPin = (uint8_t)(*((uint16_t *)pvParameters) >> 8);
  uint8_t lampNo = (uint8_t) * ((uint16_t *)pvParameters);

  readButtonInput(btnPin, lampNo);
}
void TaskButton4(void *pvParameters)
{ // This is a task.
  uint8_t btnPin = (uint8_t)(*((uint16_t *)pvParameters) >> 8);
  uint8_t lampNo = (uint8_t) * ((uint16_t *)pvParameters);

  readButtonInput(btnPin, lampNo);
}
void TaskButton5(void *pvParameters)
{ // This is a task.
  uint8_t btnPin = (uint8_t)(*((uint16_t *)pvParameters) >> 8);
  uint8_t lampNo = (uint8_t) * ((uint16_t *)pvParameters);

  readButtonInput(btnPin, lampNo);
}
void TaskButton6(void *pvParameters)
{ // This is a task.
  uint8_t btnPin = (uint8_t)(*((uint16_t *)pvParameters) >> 8);
  uint8_t lampNo = (uint8_t) * ((uint16_t *)pvParameters);

  readButtonInput(btnPin, lampNo);
}
void TaskButton7(void *pvParameters)
{ // This is a task.
  uint8_t btnPin = (uint8_t)(*((uint16_t *)pvParameters) >> 8);
  uint8_t lampNo = (uint8_t) * ((uint16_t *)pvParameters);

  readButtonInput(btnPin, lampNo);
}
void TaskButton8(void *pvParameters)
{ // This is a task.
  uint8_t btnPin = (uint8_t)(*((uint16_t *)pvParameters) >> 8);
  uint8_t lampNo = (uint8_t) * ((uint16_t *)pvParameters);

  readButtonInput(btnPin, lampNo);
}

void setup_wifi()
{
  // uint16_t timeOut = 0;
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
    // if(timeOut++ > 20) break;
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char *topic, byte *message, unsigned int length)
{
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageTemp;

  for (int i = 0; i < length; i++)
  {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  Serial.println();

  // // Feel free to add more if statements to control more GPIOs with MQTT

  // // If a message is received on the topic esp32/output, you check if the message is either "on" or "off".
  // // Changes the output state according to the message
  // if (String(topic) == "esp32/output") {
  //   Serial.print("Changing output to ");
  //   if(messageTemp == "on"){
  //     Serial.println("on");
  //     digitalWrite(ledPin, HIGH);
  //   }
  //   else if(messageTemp == "off"){
  //     Serial.println("off");
  //     digitalWrite(ledPin, LOW);
  //   }
  // }
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
      // client.subscribe("esp32/output");
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