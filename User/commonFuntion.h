#ifndef __commonFuntion_H
#define __commonFuntion_H

#include "cmsis_os.h"
#include "rtc.h"
#include <string.h>
#include <stdlib.h>

typedef enum{
	PC,
	Seed,
	GV1,
	GV2
}Gradient_Value;

typedef enum{
	Fatal_User,
	Fatal_Internal,
	NonFatal_User,
	NonFatal_Internal
}Error_Type;

typedef struct{
	Error_Type xErrType;
	portCHAR *pcTaskName;
}Error_Message;


//#define vLogToPc(pcMsg) {\
//	if( osOK != osMessagePut ( xQueueLogToPcHandle, (uint32_t)pcMsg, 0 ) ){\
//		Error_Handler(pcTaskGetTaskName(NULL));\
//	}\
//}

void vLogToPcFromISR(portCHAR *pcMsg);
void RTC_CalendarShow(RTC_HandleTypeDef RtcHandle, portCHAR *showtime, portCHAR *showdate);
void Error_Handler_Prototype(const Error_Message *pxErrMsg, unsigned portCHAR* pucFile,	unsigned portLONG ucLine);

#define random(N) (rand() / (RAND_MAX / N + 1))
#define Error_Handler(pxErrMsg) Error_Handler_Prototype(pxErrMsg, (uint8_t *)__FILE__, __LINE__)
#define min(x, y) (x >= y? y : x)

//	Dma_Gatekeeper_Exchange_Data ExData = {\
//		Dma_Tx,\
//		pcMsg\
//	};\

/*	portCHAR pcShowTime[50] = {0};\
	portCHAR pcShowDate[50] = {0};\
	portCHAR *pcMsgWithTime = NULL;\
	RTC_CalendarShow(hrtc, pcShowTime, pcShowDate);\
	pcMsgWithTime = strcat(strcat(strcat(pcShowDate, " "), strcat(pcShowTime, " ")), (pcMsg));\*/

#endif /* __commonFuntion_H */
