/*
 * analogInputs.h
 *
 *  Created on: 23.10.2024
 *      Author: A.Huth
 * 
 * Functions to get and monitor all adc values
 * 
 *    - supply voltage
 *    - ldr value 
 */

#ifndef ANALOG_H_
#define ANALOG_H_

void initADCchannels(void);

float readSuppyVoltage(void);
bool checkSuppyVoltage (float);

float readLDRvalue (void);
int getBrightnessLevels (void);

void analogInputsTask_100(void);


extern float supplyvoltage;
extern bool lowVoltage; 
extern int ldrvalue;
extern int brightnessLevel; 


#endif /* ANALOG_H_ */