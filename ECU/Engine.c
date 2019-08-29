#include "Engine.h"

void accelActuator()
{
	printf("a\n");
	switch(gear_state)
	{
		case 0:
		case 1:
			printf("Gear:%d \t[Error] Error Message \n", gear_state);
			break;
		case 2:
		case 3:
			printf("b\n");
			if(current_speed + (accel_val*10) > MAX_SPEED)
				current_speed = MAX_SPEED;
			else
				current_speed += 10 * accel_val; 
			break;
	}
	printf("c\n");
}

void breakActuator()
{
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
