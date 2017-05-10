#include "vTaskProcessManager.h"
#include "vTaskMagMeasure.h"
#include "freertos.h"
#include "usart.h"
#include "commonFuntion.h"
#include "vTaskDmaGatekeeper.h"
#include "vTaskPppHandler.h"
#include "rtc.h"

/* Queue */
extern osMessageQId xQueueLogToPcHandle;
extern osMessageQId xQueueSendComDataHandle;
extern osMessageQId xQueueReceivedComDataHandle;	/* vTaskPppHandler grenerate it */
/* Semaphore */

void vTaskProcessManager(void const * argument)
{
#ifdef DEBUG
	static unsigned portCHAR pucTaskRunningMsg[] = "\r\nMessage: Process Manager Task works well.\r\n";
	Dma_Gatekeeper_Exchange_Data pxTaskRunningMsg = {
		pucTaskRunningMsg,
		strlen((char *)pucTaskRunningMsg)
	};
	const Error_Message xErrMsgFatalInternal = {
		Fatal_Internal,
		pcTaskGetTaskName(NULL)
	};
	const Error_Message xErrMsgNonFatalInternal = {
		NonFatal_Internal,
		pcTaskGetTaskName(NULL)
	};
#endif
	

#pragma pack(1)
	static Com_Data xOneSendData = {
		{ 0, 0, 0 },
		Seed,
		0,
		0,
		0
	};
	osEvent eventReceivedComData;
	static Com_Data pxReceivedComData[_ComNum] = { 0 };
	
//	osMessageGet(xQueueReceivedComDataHandle, 0);
	/* Infinite loop */
	for (;;)
	{
		eventReceivedComData = osMessageGet(xQueueReceivedComDataHandle, 0);
		if(osEventMessage == eventReceivedComData.status){
			memcpy(pxReceivedComData, eventReceivedComData.value.p, _ComNum * _ComDataLength);
			#ifdef _DebugTaskProcessManager
			osMessageGet(xQueueSendComDataHandle, 0);
			memcpy(&xOneSendData, &(pxReceivedComData[GV1]), _ComDataLength);
			osMessagePut(xQueueSendComDataHandle, (uint32_t)&xOneSendData, 0);
			#endif
		}

#ifdef DEBUG
		/* log running message */
		osMessagePut(xQueueLogToPcHandle, (uint32_t)&pxTaskRunningMsg, 0);

#ifdef _DebugTaskProcessManager
//		/* produce one send data */
//		osMessageGet(xQueueSendComDataHandle, 0);
//		if (osOK != HAL_RTC_GetTime(&hrtc, &xOneSendData.xTimestamp, RTC_FORMAT_BCD)) {
//			Error_Handler(&xErrMsgNonFatalInternal);
//		}
//		xOneSendData.ulOrientation = 0x1020307E;
//		xOneSendData.ulVelocity = 0x5060707D;
//		xOneSendData.ulAcceleration = 0x90A0B0C0;
//		if (osOK != osMessagePut(xQueueSendComDataHandle, (uint32_t)&xOneSendData, 0)){
//			Error_Handler(&xErrMsgNonFatalInternal);
//		}
//		/* delay */
//		if (osEventTimeout != osDelay(random(5000))){
//			//			Error_Handler(pcTaskGetTaskName(NULL));
//		}
#endif
#endif
		osThreadYield();
	}
}
