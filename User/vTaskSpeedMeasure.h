#ifndef __vTaskSpeedMeasure_H
#define __vTaskSpeedMeasure_H

#include "commonFuntion.h"
#define _DebugTaskSpeedMeasure

#pragma pack(1)
typedef struct{
	Polar_Vector xVelocity;
}Speed_Measure_Data;
#pragma pack()

void vTaskSpeedMeasure(void const * argument);

#endif /*__vTaskSpeedMeasure_H */
