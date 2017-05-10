#ifndef __vTaskPppHandler_H
#define __vTaskPppHandler_H

#include "cmsis_os.h"
#include "commonFuntion.h"
#include "rtc.h"

//#define _DebugTaskPppHandler


//#define _ReceivedComDataSize (_ReceivedComDataBufferSize * 5)

#pragma pack(1)
typedef struct{
	RTC_TimeTypeDef		xTimestamp;
	Gradient_Value	xGv;
	unsigned portLONG		ulOrientation;
	unsigned portLONG		ulVelocity;
	unsigned portLONG		ulAcceleration;
}Com_Data;
#define _ComDataLength (sizeof(Com_Data))
#define _ZigbeeHeaderLength 4/* B */
#define _ZigbeeFrameMaxLength 32/* B */
#define _PppSleepRandomTimeMax 5000/* ms */

#define _ReceivedComDataBufferSize (_ComDataLength * 2)

void vTaskPppHandler(void const * argument);
void ppp_upcallX(portCHAR *buffer, portSHORT len);

#endif /*__vTaskPppHandler_H */
