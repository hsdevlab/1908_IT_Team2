#include "FileManager.h"

// void main(){
// 	loadFile();
// 	increaseDistance();
// 	decreaseFuel();
// }
void loadFile()
{
	if((fp = fopen("carinfo.txt", "r")) == NULL){
		printf("Create New carinfo.txt file..");
		fp = fopen("carinfo.txt", "w");
		fprintf(fp, "%d %d", INIT_FUEL, INIT_DISTANCE);
		current_fuel = INIT_FUEL;
		current_total_distance = INIT_DISTANCE;
		return;
	}
	else{
		fscanf(fp, "%02d %00005d", &current_fuel, &current_total_distance);
		fclose(fp);
		fp = fopen("carinfo.txt", "w");
	}
	fseek(fp, 0, SEEK_SET);
}

void increaseDistance()
{
	current_total_distance += 1;
	fprintf(fp, "%02d %00005d", current_fuel, current_total_distance);
	fseek(fp, 0, SEEK_SET);
}

void decreaseFuel()
{
	current_fuel -= 1;
	fprintf(fp, "%02d %00005d", current_fuel, current_total_distance);
	fseek(fp, 0, SEEK_SET);
}