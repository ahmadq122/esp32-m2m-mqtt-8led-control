## esp32-m2m-mqtt-8led-control
# ESP32 M2M MQTT 8 LEDs CONTROL

**Publisher**
Controller: ESP32 Dev Kit
Input: 8 Pushbuttons (internally pulled up)
Output: JsonData published to MQTT broker

JsonData: {"led1":0, "led2":0, "led3":0, "led4":0, "led5":0, "led6":0, "led7":0, "led8":0}
* The value of each key is an 8-bit integer in the range 0-100.
* The value of the key consists of 3 state of values namely 0, 50 and 100.
* Click the pushbutton once to switch the state of the value
* 0 will be the initial value
* By clicking the pushbutton will switch the value to 50, click again to switch to 100.
* At 100 click the pushbutton to reset the value to 0
* Push Button numbering is as follows

  -- 1 -- 2 --

  -- 3 -- 4 --

  -- 5 -- 6 --

  -- 7 -- 8 --

![image](https://github.com/ahmadq122/esp32-m2m-mqtt-8led-control/assets/74335302/69255fdc-be57-4a12-aa25-0568be2cdf41)

**Subscriber**
Controller: ESP32 Dev Kit
Output: 8 LEDs
Input: JsonData subscribed in MQTT broker

JsonData: {"led1":0, "led2":0, "led3":0, "led4":0, "led5":0, "led6":0, "led7":0, "led8":0}
* The value of each key will be used as the intensity value of each LED
* LEDs numbering is as follows

-- 1 -- 2 -- 3 -- 4 -- 5 -- 6 -- 7 -- 8 --
  
![image](https://github.com/ahmadq122/esp32-m2m-mqtt-8led-control/assets/74335302/3e96e4d8-05a9-427c-8e9f-0ff540258520)

_Powered by: VS Code, Platform IO, Wokwi, and Arduino framework_
