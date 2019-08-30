#ifndef __ENGINE_H__
#define __ENGINE_H__

#include <stdio.h>
#define MAX_SPEED 240
#define MAX_RPM   8

extern int gear_state; // 0: P, 1: N, 2: R, 3: D
extern int accel_val; // 0, 1, 2, 3
extern int break_val; // 0, 1, 2, 3
extern int wink_state; // 0: None, 1: Left, 2: right, 3: Warning
extern int current_speed;
extern int RPM;

void accelActuator();
void breakActuator();
void nonActuator(); // Dicrease speed in natural

#endif
