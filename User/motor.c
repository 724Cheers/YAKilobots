#include"interface.h"
#include "gpio.h"
extern unsigned int speed_count;//占空比计数器 50次一周期
extern char left_speed_duty;
extern char right_speed_duty;

void CarGo()
{
	LEFT_UP;
	RIGHT_UP;	
}
	
void CarBack()
{
	LEFT_DOWN;
	RIGHT_DOWN;	
}

void CarStop()
{
	LEFT_STOP;
	RIGHT_STOP;
}



