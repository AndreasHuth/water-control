# Watering System
## Short Description

> [!IMPORTANT]
> The project is still in progress !!!

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
  - tzapu/WiFiManager
  - bblanchon/ArduinoJson @ 5.13.4
  - knolleary/PubSubClient @ ^2.8
  - blackhack/LCD_I2C @ ^2.3.0

## TODOs

- [x] PCB testing
   * failures due to wrong PIN mapping (ACD2 is not working with wifi!) :-(
   * LDR will NOT work.
   * Jumper J2 /J3 / J4 will not work due to missing pull up (ESP32 does not support)
- [ ] Check supply concept
- [ ] Sensor testing
- [ ] Implement / check OTA
- [ ] Display control / UI 
- [ ] implement main function 
    * turn on / off incl main ventil
    * Control by button & MQTT
    * Specification of the water quantity & regulate water quantity 
    * button before mqtt 
- [ ] define all saftey features
  * turn off by 
    * no wifi connection
    * no mqtt broker 
    * time out 
    * Limit the amount of water
  * MQTT QoS2 !
- [ ] implement minor features
  * temperature measurement
  * LDR / light function - cancelled, not supported by HW

- [ ] if the proof of function is successful, design the housing  :-) 


## ISSUES 
- LDR / light function - cancelled, not supported by HW
- Jumper J2 /J3 / J4 will not work due to missing pull up resistors


## Links
tbd.


## License

