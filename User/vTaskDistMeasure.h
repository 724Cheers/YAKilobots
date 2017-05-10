#ifndef __vTaskDistMeasure_H
#define __vTaskDistMeasure_H

#include "commonFuntion.h"
//#define _DebugTaskDistMeasure

#pragma pack(1)
typedef struct{
	Polar_Vector xPosition1Min;
	Polar_Vector xPosition1Max;
	Polar_Vector xPosition2Min;
	Polar_Vector xPosition2Max;
}Dist_Measure_Data;
#pragma pack()

void vTaskDistMeasure(void const * argument);

#endif /*__vTaskDistMeasure_H */
