#include "stdlib.h"


#ifndef __MOTOR_H
#define __MOTOR_H

void pid_init(void);
float PID_realize(float speed);

typedef struct
{
  float SetSpeed;//速度期望值
  float ActualSpeed;//速度实际值
	float err;//偏差值
	float err_last;//上一个偏差值
	float Kp,Ki,Kd;//比例、微分、积分舷凳?
	float voltage;//电压值，和pwm有关
  float integral;//定义积分值
}PID;

void CarMove(void);
void CarGo(void);
void CarBack(void);
void CarLeft(void);
void CarRight(void);
void CarStop(void);


#endif
