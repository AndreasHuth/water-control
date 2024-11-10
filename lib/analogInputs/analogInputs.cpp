/*
 * analogInputs.cpp
 *
 *  Created on: 25.10.2024
 *      Author: A. Huth
 */
#include <Arduino.h>
#include "../../src/define.h"
#include "analogInputs.h"

float supplyvoltage = 12.0;
bool lowVoltage = true;

#define CONVERSION_VALUE 6.0    // 100kOhm / 20kOhm  >> 120/20 = 6

//#define DEBUG 1
#define RATIO 0.90

void initADCchannels(void)
{
    pinMode(PIN_UIN, INPUT);
}

void calcSuppyVoltage(void)
{
    static float old_supply_value = 12.0;
    float adcvalue = (float)analogRead(PIN_UIN) * CONVERSION_VALUE / 1000.0;

    supplyvoltage = (1 - RATIO) * adcvalue + RATIO * old_supply_value;
    old_supply_value = supplyvoltage;

#if defined(DEBUG)
    Serial.print("adc: ");
    Serial.println(adcvalue);
    Serial.print("voltage : ");
    Serial.println(supplyvoltage);
#endif
}

float readSuppyVoltage(void)
{
    return supplyvoltage;
}

bool checkSuppyVoltage(void)
{
    return lowVoltage;
}

void checkVoltage(float reference)
{
    static int lowSupplyVoltageCounter = 0;
    bool lowVoltage;

    if (supplyvoltage < reference)
    {
        if (lowSupplyVoltageCounter < 5)
            lowSupplyVoltageCounter++;
    }
    else
    {
        if (lowSupplyVoltageCounter > 0)
            lowSupplyVoltageCounter--;
    }

    if (lowSupplyVoltageCounter == 5)
        lowVoltage = true;
    else
        lowVoltage = false;

#if defined(DEBUG)
    Serial.print("reference: ");
    Serial.println(reference);

    Serial.print("lowSupplyVoltageCounter : ");
    Serial.println(lowSupplyVoltageCounter);

    Serial.print("low supply : ");
    Serial.println(lowVoltage);
#endif
}

void analogInputsTask_100(void)
{
    calcSuppyVoltage();
    checkVoltage(LOWBATTERY);
}
