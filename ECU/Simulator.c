#include "Simulator.h"

void accelActuator()
{
	switch(gear)
	{
		case P:
			// TODO : Cannot increaseSpeed. 
			break;
		case R:
			// TODO : increaseSpeed, activateBreak;
			break;
		case N:
			// TODO : ?
			break;
		case D:
			// TODO : increaseSpeed, activateBreak;
			break;
	}
}

void breakActuator()
{
	
}

void increaseSpeed()
{

}

void activateBreak()
{

}

void updateTotalDistance()
{

} 

void useFuel()
{
	current_fuel -= 0.05;
	fprintf(fpFuel, "%lf", current_fuel);
}