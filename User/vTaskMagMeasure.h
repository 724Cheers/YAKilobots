#ifndef __vTaskMagMeasure_H
#define __vTaskMagMeasure_H

#define _DebugTaskMagMeasure

#define MPU9250

#pragma pack(1)
typedef struct{
	double dDirection;
}Mag_Measure_Data;
#pragma pack()

void vTaskMagMeasure(void const * argument);

#endif /*__vTaskMagMeasure_H */
