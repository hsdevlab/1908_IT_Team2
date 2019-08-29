#ifndef __FILEMANAGER_H__
#define __FILEMANAGER_H__

#include <stdio.h>
#define INIT_FUEL 50
#define INIT_DISTANCE 0
#define FUEL_IFFICIENCY 20
extern int current_fuel;
extern int current_total_distance;
extern FILE *fp; // has fuel and total distance.

void loadFile();
void increaseDistance();
void decreaseFuel();

#endif