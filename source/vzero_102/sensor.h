#ifndef SENSOR_H
#define SENSOR_H

/*
 *  Project: Volta_Zero
 *  Creation date: 24/12/2018
 *  Version: 1.0.0 
 *  Last update: 20/01/2019
 *  Authors: S. Lafi, A. Elzayat
 *  License: MIT -- No Warranties
 */
 
#include "Arduino.h"

float getOnboardTemperatureValue(const int onboard_ts_pin);
float getExternalTemperatureValue(const int temp_k_type_pin);
float getLightSensorValue(const int lightsensor_pin);
void blink(int mode);

#endif
