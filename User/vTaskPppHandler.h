#ifndef __vTaskPppHandler_H
#define __vTaskPppHandler_H

#include "cmsis_os.h"
#include "commonFuntion.h"
#include "rtc.h"

#define _ReceivedDataBufferSize _ZigbeeRxSize

typedef struct{
	RTC_TimeTypeDef		xTimestamp;
	Gradient_Value	xGv;
	unsigned portLONG		ulOrientation;
	unsigned portLONG		ulVelocity;
	unsigned portLONG		ulAcceleration;
}Com_Data;

void vTaskPppHandler(void const * argument);
void ppp_upcallX(portCHAR *buffer, portSHORT len);

#endif /*__vTaskPppHandler_H */
