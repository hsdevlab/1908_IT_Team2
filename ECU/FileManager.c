#include "FileManager.h"

extern int current_fuel;
extern int current_total_distance;
extern FILE *fpinfo; // has fuel and total distance.

// void main(){
// 	loadFile();
// 	increaseDistance();
// 	decreaseFuel();
// }
void loadFile()
{
	if((fpinfo = fopen("carinfo.txt", "r")) == NULL){
		printf("Create New carinfo.txt file..");
		fpinfo = fopen("carinfo.txt", "w");
		current_fuel = INIT_FUEL;
		current_total_distance = INIT_DISTANCE;
		fprintf(fpinfo, "%02d %00005d", current_fuel, current_total_distance);
		printf("[INIT_LOADFILE] fuel : %d, current_total_distance:%d\n", current_fuel, current_total_distance);
		return;
	}
	else{
		fscanf(fpinfo, "%02d %00005d", &current_fuel, &current_total_distance);
		fclose(fpinfo);
		fpinfo = fopen("carinfo.txt", "w");
		fprintf(fpinfo, "%02d %00005d", current_fuel, current_total_distance);
		printf("[LOADFILE] fuel : %d, current_total_distance:%d\n", current_fuel, current_total_distance);
	}

	fseek(fpinfo, 0, SEEK_SET);
}

void increaseDistance()
{
	current_total_distance += 1;
	fprintf(fpinfo, "%02d %00005d", current_fuel, current_total_distance);
	fseek(fpinfo, 0, SEEK_SET);
}

void decreaseFuel()
{
	current_fuel -= 1;
	fprintf(fpinfo, "%02d %00005d", current_fuel, current_total_distance);
	fseek(fpinfo, 0, SEEK_SET);
}