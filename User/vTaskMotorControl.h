#ifndef __vTaskMotorControl_H
#define __vTaskMotorControl_H

#include "commonFuntion.h"
#define _DebugTaskMotorControl

#pragma pack(1)
typedef struct{
	Polar_Vector xPreDisplacement;
	Polar_Vector xRealDisplacement;
}Motor_Control_Data;
#pragma pack()

void vTaskMotorControl(void const * argument);

#endif /*__vTaskMotorControl_H */
