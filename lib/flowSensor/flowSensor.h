/*
 * flowSensor.h
 *
 *  Created on: 23.10.2024
 *      Author: A.Huth
 */

#ifndef FLOWSENSOR_H_
#define FLOWSENSOR_H_

void initFlowSensor(void);

void flowSensorRead(unsigned long  calibration);
unsigned long getTotalPulse(void);
float getFlowRate_h(void);
float getFlowRate_m(void);
float getFlowRate_s(void);
float getSensorVolume(void);
void resetPulse(void);
void resetSensorVolume(void);


#endif /* FLOWSENSOR_H_ */