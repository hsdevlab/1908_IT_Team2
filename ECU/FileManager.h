#ifndef __FILEMANAGER_H__
#define __FILEMANAGER_H__

#include <stdio.h>
#define INIT_FUEL 50
#define INIT_DISTANCE 0
static int current_fuel;
static int current_total_distance;
FILE *fp; // has fuel and total distance.

void loadFile();
void increaseDistance();
void decreaseFuel();

#endif