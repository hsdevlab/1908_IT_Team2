#ifndef __SIMULATOR_H__
#define __SIMULATOR_H__
#include <stdio.h>

typedef enum Gear = { P, R, N, D } GEAR;

static GEAR gear = P;
static int accel_val 	= 0; // 0, 1, 2, 3
static int break_val	= 0; // 0, 1, 2, 3 
static int current_speed;
static double current_fuel;
FILE *fpFuel, *fpTotalDistance; 
fpFuel = fopen("fuel.txt", "r+");
if(fpFuel == NULL){
	printf("Create New fuel.txt file.\n");
	fpFuel = fopen("fuel.txt", "w+");
	fprintf(fp, "50.0");
}
fscanf(fpFuel, "%lf", &current_fuel);
fpFuel = fopen("fuel.txt", "w");


void accelActuator();
void breakActuator();
void increaseSpeed();
void activateBreak();
void updateTotalDistance();
void useFuel();

#endif