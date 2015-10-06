# pubnub-esp8266-rest-api

In this repo esp8266 is used to publish and subscribe to PUBNUB using the REST API without any expternal chip and the esp8266 is programmed using the Arduino IDE


### Installing the Arduino ESP8266 Core with the Boards Manager ###

- Download and Install Arduino 1.6.5 from [https://www.arduino.cc/en/Main/Software]
- Start Arduino and open Preferences window.
- Enter ```http://arduino.esp8266.com/stable/package_esp8266com_index.json``` into *Additional Board Manager URLs* field. You can add multiple URLs, separating them with commas.
- Open Boards Manager from Tools > Board menu and install *esp8266* platform (and don't forget to select your ESP8266 board from Tools > Board menu after installation).

### Uploading the Program to the ESP8266 using the Arduino IDE

Step 1: Get this Git Repo to your desktop using,

                git clone https://github.com/suryasundarraj/pubnub-esp8266-rest-api.git

Step 2: Open the codes in Arduino IDE

Step 3: Select the NodeMCU 1.0(ESP8266 V12-E), 80Mhz,115200

Step 4: Select the USB Port from Tools - > Port

Step 5: Edit the SSID and PASSWORD to configure to your router

Step 6: Edit the pubnub publish and subscribe keys to your unique key provided by pubnub.com

Step 7: Upload the Code to the ESP8266 (Note: Connect the GPIO 0 to GND to Pull the ESP8266 to Flashing Mode)
