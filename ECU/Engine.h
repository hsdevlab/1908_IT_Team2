#ifndef __ENGINE_H__
#define __ENGINE_H__

#include <stdio.h>
#define MAX_SPEED 240
#define MAX_RPM   240

void accelActuator();
void breakActuator();
void nonActuator(); // Dicrease speed in natural

#endif
