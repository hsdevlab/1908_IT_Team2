#include "Engine.h"

void accelActuator()
{
	RPM = RPM+1100;
	if(RPM>8000)
	{
		RPM = 8000;
	}

	switch(gear_state)
	{
		case 0:
		case 1:
			printf("Gear:%d \t[Error] Error Message \n", gear_state);
			break;
		case 2:
		case 3:
			if(current_speed + (accel_val*10) > MAX_SPEED)
				current_speed = MAX_SPEED;
			else
				current_speed += 10 * accel_val; 
			break;
	}
}

void breakActuator()
{
	RPM = RPM-1500;
	if(RPM<0)
	{
		RPM = 0;
	}

	switch(gear_state)
	{
		case 0:
		case 1:
			break;
		case 2:
		case 3:
			if(current_speed - (20*break_val) < 0)
				current_speed = 0;
			else
				current_speed -= 20 * break_val; 
			break;
	}
}

void nonActuator()
{
	RPM = RPM-200;
	if(RPM<0)
	{
		RPM = 0;
	}

	switch(gear_state)
	{
		case 0:
		case 1:
			break;
		case 2:
		case 3:
			if(current_speed - 2 < 0)
				current_speed = 0;
			else
				current_speed -= 2; 
			break;
	}
}
