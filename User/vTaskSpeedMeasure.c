#include "vTaskSpeedMeasure.h"
#include "cmsis_os.h"
#include "commonFuntion.h"
#include "vTaskDmaGatekeeper.h"

/* Queue */
extern osMessageQId xQueueLogToPcHandle;
extern osMessageQId xQueueSpeedMeasureDataHandle;	/* save data */
extern osMessageQId xQueueRunTokenSpeedMeasureHandle;	/* execute token */
/* Semaphore */

void vTaskSpeedMeasure(void const * argument)
{
#ifdef DEBUG
	const unsigned portCHAR pucTaskRunningMsg[] = "\r\nMessage: Speed Measure Task works well.\r\n";
	Dma_Gatekeeper_Exchange_Data pxTaskRunningMsg = {
		(unsigned char *)pucTaskRunningMsg,
		strlen((char *)pucTaskRunningMsg)
	};
#endif
	
#pragma pack(1)
	static Speed_Measure_Data xCurrentSpeedMeasureData = {0};
#pragma pack()
		
	/* Infinite loop */
	for (;;)
	{
		osMessagePeek(xQueueRunTokenSpeedMeasureHandle, osWaitForever);
		/* add codes here */
		
#ifdef DEBUG
		/* log running message */
		osMessagePut(xQueueLogToPcHandle, (uint32_t)&pxTaskRunningMsg, 0);		
		
		#ifdef _DebugTaskSpeedMeasure
		osDelay(random(5000));
		osMessagePut(xQueueSpeedMeasureDataHandle, (uint32_t)&xCurrentSpeedMeasureData, osWaitForever);
		#endif		
#endif
		osThreadYield();
	}
}
