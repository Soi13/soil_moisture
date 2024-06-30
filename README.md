Measure soil moisture, temperarure and humidity with ESP-8266-WiFi controller and transmit data to Home Assistant via MQTT protocol.

Libs:
DHT11 by Author: Dhruba Saha
PubSubClient by Nick 'O Leary

For connecting to HA via MQTT:
https://www.home-assistant.io/blog/2015/10/11/measure-temperature-with-esp8266-and-report-to-mqtt/

How to connect/programm ESP8266:
https://www.instructables.com/NODEMcu-Usb-Port-Not-Working-Upload-the-Code-Using/

ESP8266 Pinout Reference:
https://randomnerdtutorials.com/esp8266-pinout-reference-gpios/

Example of connecting:
https://lastminuteengineers.com/soil-moisture-sensor-arduino-tutorial/#google_vignette


=== In simple terms ===
When you are ready to upload your code to the board
Connect (Serial Converter -> NodeMCU board):
[1] RX -> TX
[2] TX -> RX
[3] 5v -> VIN (not 3.3v to VIN, but 5v to VIN, notice no ambiguity there)
[4] GND -> GND
[5] Button Sequence: Hold down the Flash button and tap and release Rst button, then after about 1sec release Flash (read that twice) - be sure to keep the Flash button down while you first release the Rst button (if you do not do this, you will not put the board into upload/programming mode. As far as I can tell, it is safe to say that when you see the blue led flash, you can release the Flash button - but only knowing the wiring of the NodeMCU can you know for sure. Remember, the release of the Rst button causes the reset/start of the board.

After the upload/programming is completed, your NodeMCU program should begin to execute - if not, press the Rst button to restart to brak out of programming mode. If you want to upload again you must follow the steps again as the board is no longer in upload/programming mode. A Wemos Esp-8266 will not work this way as there is no Flash button as there in on a NodeMCU board.
===========================

