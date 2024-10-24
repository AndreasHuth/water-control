/*
 * buttons.cpp
 *
 *  Created on: 23.10.2024
 *      Author: A. Huth
 */
#include <Arduino.h>
#include "../../src/define.h"
#include "buttons.h"
// #define DEBUG 1
#define RATIO 0.90

#if defined(DEBUG)
Serial.println(F("Init sensor!"));
#endif

bool button1event = false;
bool button2event = false;
bool button3event = false;

// Checks if motion was detected, sets LED HIGH and starts a timer
void IRAM_ATTR Button1Event()
{
    // digitalWrite(PIN_BUTTON_1, HIGH);
    button1event = true;

}

void IRAM_ATTR Button2Event()
{
    // digitalWrite(PIN_BUTTON_1, HIGH);
    button2event = true;

}

void IRAM_ATTR Button3Event()
{
    // digitalWrite(PIN_BUTTON_1, HIGH);
    button3event = true;

}

void initButtons(void)
{
    // PIR Motion Sensor mode INPUT_PULLUP
    pinMode(PIN_BUTTON_1, INPUT_PULLUP);
    // Set motionSensor pin as interrupt, assign interrupt function and set RISING mode
    attachInterrupt(digitalPinToInterrupt(PIN_BUTTON_1), Button1Event, FALLING);
    // PIR Motion Sensor mode INPUT_PULLUP
    pinMode(PIN_BUTTON_2, INPUT_PULLUP);
    // Set motionSensor pin as interrupt, assign interrupt function and set RISING mode
    attachInterrupt(digitalPinToInterrupt(PIN_BUTTON_2), Button2Event, FALLING);
    // PIR Motion Sensor mode INPUT_PULLUP
    pinMode(PIN_BUTTON_3, INPUT_PULLUP);
    // Set motionSensor pin as interrupt, assign interrupt function and set RISING mode
    attachInterrupt(digitalPinToInterrupt(PIN_BUTTON_3), Button3Event, FALLING);
}

void buttonStatus(void)
{
    bool button1 = digitalRead(PIN_BUTTON_1);
    bool button2 = digitalRead(PIN_BUTTON_2);
    bool button3 = digitalRead(PIN_BUTTON_3);

    Serial.print("button 1 : ");
    Serial.println(button1);
    Serial.print("button 2 : ");
    Serial.println(button2);
    Serial.print("button 3 : ");
    Serial.println(button3);
}

bool getButton1(void)
{
    bool helper = (bool)!digitalRead(PIN_BUTTON_1);
    return helper;
}

bool getButton2(void)
{
    bool helper = (bool)!digitalRead(PIN_BUTTON_2);
    return helper;
}

bool getButton3(void)
{
    bool helper = (bool)!digitalRead(PIN_BUTTON_2);
    return helper;
}
