#include "vTaskMagMeasure.h"
#include "freertos.h"
#include "usart.h"
#include "commonFuntion.h"
#include "vTaskDmaGatekeeper.h"
#include "vTaskPppHandler.h"
#include "rtc.h"

/* Queue */
extern osMessageQId xQueueLogToPcHandle;
extern osMessageQId xQueueSendComFrameHandle;
/* Semaphore */

void vTaskProcessManager(void const * argument)
{
	#ifdef DEBUG
	unsigned portCHAR pucTaskRunningMsg[] = "\r\nMessage: Process Manager Task works well.\r\n";
	Dma_Gatekeeper_Exchange_Data pxTaskRunningMsg = {
		pucTaskRunningMsg,
		strlen((char *)pucTaskRunningMsg)
	};
	#endif
	
	#pragma pack(1)
	Com_Data xOneSendData = {
		{0,0,0},
		Seed,
		0,
		0,
		0
	};

  /* Infinite loop */
  for(;;)
  {
		
		#ifdef DEBUG
		if( osOK != HAL_RTC_GetTime  ( &hrtc,    &xOneSendData.xTimestamp,    RTC_FORMAT_BCD   )) {
			Error_Handler();
		}
		if( osOK != osMessagePut ( xQueueSendComFrameHandle, (uint32_t)&xOneSendData, 0 ) ){
			Error_Handler();
		}

		if( osOK != osMessagePut ( xQueueLogToPcHandle, (uint32_t)&pxTaskRunningMsg, 0 ) ){
			Error_Handler();
		}
		if( osEventTimeout != osDelay(5000) ){
//			Error_Handler();
		}
		#endif
		osThreadYield();
  }
}
