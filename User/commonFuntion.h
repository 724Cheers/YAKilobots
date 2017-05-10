#ifndef __commonFuntion_H
#define __commonFuntion_H

#include "cmsis_os.h"

//#define vLogToPc(pcMsg) ( osMessagePut ( \
//	xQueueLogToPcHandle, \
//	(uint32_t)&{ Dma_Tx, pcMsg }, \
//	osWaitForever ) );

//void vLogToPc(portCHAR *pcMsg);
void vLogToPcFromISR(portCHAR *pcMsg);
void Error_Handler(void);

#endif /* __commonFuntion_H */
