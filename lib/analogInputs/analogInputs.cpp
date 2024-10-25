/*
 * analogInputs.cpp
 *
 *  Created on: 25.10.2024
 *      Author: A. Huth
 */
#include <Arduino.h>
#include "../../src/define.h"
#include "analogInputs.h"

int ldrvalue;
int brightnessLevel = 0;

float supplyvoltage = 0.0;
bool lowVoltage = true;

#define CONVERSION_VALUE 5    // 200kOhm / 50kOhm  >> 250/50 = 5
#define BRIGHTNESS_FACTOR 409 //

// #define DEBUG 1
#define RATIO 0.90

#if defined(DEBUG)
Serial.println(F("Init sensor!"));
#endif

void initADCchannels(void)
{
    pinMode(PIN_LDR, INPUT);
    pinMode(PIN_UIN, INPUT);
}

void calcSuppyVoltage(void)
{
    static float old_supply_value = 12.0;
    float adcvalue = (float)analogRead(PIN_UIN) * CONVERSION_VALUE / 1000.0;

    supplyvoltage = (1 - RATIO) * adcvalue + RATIO * old_supply_value;
    old_supply_value = supplyvoltage;

    Serial.print("adc: ");
    Serial.println(adcvalue);
    Serial.print("voltage : ");
    Serial.println(supplyvoltage);
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

    Serial.print("reference: ");
    Serial.println(reference);

    Serial.print("lowSupplyVoltageCounter : ");
    Serial.println(lowSupplyVoltageCounter);

    Serial.print("low supply : ");
    Serial.println(lowVoltage);
}

void calcLDRvalue(void)
{
    static float old_LDR_value = 0.0;

    float adcvalue = (float)analogRead(PIN_LDR);

    ldrvalue = (1 - RATIO) * adcvalue + RATIO * old_LDR_value;
    old_LDR_value = ldrvalue;

    Serial.print("adc: ");
    Serial.println(adcvalue);
    Serial.print("ldrvalue : ");
    Serial.println(ldrvalue);
}

float readLDRvalue(void)
{
    return ldrvalue;
}

int getBrightnessLevels(void)
{
    brightnessLevel = ldrvalue / BRIGHTNESS_FACTOR;
    return brightnessLevel;
}

void analogInputsTask_100(void)
{
    calcLDRvalue();
    getBrightnessLevels();

    calcSuppyVoltage();
    checkVoltage(LOWBATTERY);
}
