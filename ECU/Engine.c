#include "Engine.h"

void accelActuator()
{
	switch(gear)
	{
		case P:
		case N:
			printf("[Error] Error Message \n");
			break;
		case R:
		case D:
			if(current_speed + (accel_val*10) > MAX_SPEED)
				current_speed = MAX_SPEED;
			else
				current_speed += 10 * accel_val; 
			break;
	}
}

void breakActuator()
{
	switch(gear)
	{
		case P:
		case N:
			break;
		case R:
		case D:
			if(current_speed - (20*break_val) < 0)
				current_speed = 0;
			else
				current_speed -= 20 * break_val; 
			break;
	}
}

void nonActuator()
{
	switch(gear)
	{
		case P:
		case N:
			break;
		case R:
		case D:
			if(current_speed - 2 < 0)
				current_speed = 0;
			else
				current_speed -= 2; 
			break;
	}
}
