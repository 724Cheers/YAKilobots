#ifndef __commonFuntion_H
#define __commonFuntion_H

#include "cmsis_os.h"
#include "rtc.h"
#include <string.h>

#define vLogToPc(pcMsg) {\
	if( osOK != osMessagePut ( xQueueLogToPcHandle, (uint32_t)pcMsg, 0 ) ){\
		Error_Handler();\
	}\
}

void vLogToPcFromISR(portCHAR *pcMsg);
void RTC_CalendarShow(RTC_HandleTypeDef RtcHandle, portCHAR *showtime, portCHAR *showdate);
void Error_Handler(void);

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
