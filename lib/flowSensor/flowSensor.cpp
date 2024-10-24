/*
 * flowSensor.cpp
 *
 *  Created on: 23.10.2024
 *      Author: A. Huth
 */
#include <Arduino.h>
#include "../../src/define.h"
#include "flowSensor.h"

// #define DEBUG 1


// #define YFS201	450
// #define YFB1	660

#if defined(DEBUG)
Serial.println(F("Init sensor!"));
#endif

unsigned long pulse;
unsigned long totalpulse;

unsigned long pulse1liter = 396; // Inet: 450
unsigned long timebefore = 0;
float flowratehour = 0;
float flowrateminute = 0;
float flowratesecound = 0;
float SensorVolume = 0;

// Checks if motion was detected, sets LED HIGH and starts a timer
void IRAM_ATTR FlowSensorEvent()
{
    pulse= pulse+1;
}

void initFlowSensor(void)
{
    // PIR Motion Sensor mode INPUT_PULLUP
    pinMode(PIN_SENSOR, INPUT_PULLUP);
    // Set motionSensor pin as interrupt, assign interrupt function and set RISING mode
    attachInterrupt(digitalPinToInterrupt(PIN_SENSOR), FlowSensorEvent, RISING);
}

void flowSensorRead(long calibration)
{
    flowratesecound = (pulse / (pulse1liter + calibration)) / ((millis() - timebefore) / 1000);
    SensorVolume += (pulse / (pulse1liter + calibration));
    totalpulse += pulse;
    pulse = 0;
    timebefore = millis();
}

unsigned long getTotalPulse(void)
{
    return totalpulse;
}

float getFlowRate_h(void)
{
    flowratehour = flowratesecound * 3600;
    return flowratehour;
}

float getFlowRate_m(void)
{
    flowrateminute = flowratesecound * 60;
    return flowrateminute;
}

float getFlowRate_s(void)
{
    return flowratesecound;
}

float getSensorVolume(void)
{
    return SensorVolume;
}

void resetPulse(void)
{
    pulse = 0;
    totalpulse = 0;
}

void resetSensorVolume(void)
{
    SensorVolume = 0;
}
