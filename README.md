# Watering System
## The project is still in progress !!!
## Short Description
Project to control water quantities for watering plants. 
Control of an main and control valve via: 
* buttons
* MQTT - Wifi

Display to show the status and data.

## How to use
Please download the whole project and open it direktly in Visual Studio Code. 
PlatformIO Extension is necessary.

HINT: this project is still in development 10/2024!

## further informations
Needed packages:
  - tzapu/WiFiManager @ ^0.16.0
  - bblanchon/ArduinoJson @ 5.13.4
  - knolleary/PubSubClient @ ^2.8
  - blackhack/LCD_I2C @ ^2.3.0

## TODOs

[ ] PCB testing

[ ] Check supply concept
[ ] Sensor testing
[ ] Implement / check OTA
[ ] Display control / UI 
[ ] implement main function 
  - turn on / off incl main ventil
  * Control by button & MQTT
  * Specification of the water quantity & regulate water quantity 
  * button before mqtt 
[ ] define all saftey features
  * turn off by 
    * no wifi connection
    * no mqtt broker 
    * time out 
    * Limit the amount of water
  * MQTT QoS2 !
* implement minor features
  * temperature measurement
  * LDR / light function

* if the proof of function is successful, design the housing  :-) 


## Links
tbd.


## License

