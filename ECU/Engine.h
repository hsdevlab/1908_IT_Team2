#ifndef __ENGINE_H__
#define __ENGINE_H__

#include <stdio.h>
typedef enum Gear{ P, R, N, D } GEAR;
#define MAX_SPEED 300

static GEAR gear = P;
static int accel_val 	 = 0; // 0, 1, 2, 3
static int break_val	 = 0; // 0, 1, 2, 3 
static int current_speed = 0;

void accelActuator();
void breakActuator();
void nonActuator(); // Dicrease speed in natural

#endif