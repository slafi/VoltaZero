#ifndef UTILS_H
#define UTILS_H

/*
 *  Project: Volta_Zero
 *  Creation date: 24/12/2018
 *  Version: 1.0.0 
 *  Last update: 12/03/2019
 *  Author: S. Lafi
 *  Copyright: VWT (c) 2018-2019
 *  License: MIT -- No Warranties
 *
 */

#include "Arduino.h"
#include "MemoryFree.h"

/* Declare automatic reset function */

char* formatMessage(int id, float temp0,float temp1, int sensStat, int bzStatus);
char* formatMessage(int id, float temp0,float temp1, double sensitivity, int bzStatus);
void checkMemory();

#endif
