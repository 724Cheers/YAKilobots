#include "vTaskDistMeasure.h"
#include "cmsis_os.h"
#include "commonFuntion.h"
#include "vTaskDmaGatekeeper.h"

/* Queue */
extern osMessageQId xQueueLogToPcHandle;
extern osMessageQId xQueueDistMeasureDataHandle;	/* save data */
extern osMessageQId xQueueRunTokenDistMeasureHandle;	/* execute token */
/* Semaphore */

void vTaskDistMeasure(void const * argument)
{
#ifdef DEBUG
	const unsigned portCHAR pucTaskRunningMsg[] = "\r\nMessage: Dist Measure Task works well.\r\n";
	Dma_Gatekeeper_Exchange_Data pxTaskRunningMsg = {
		(unsigned char *)pucTaskRunningMsg,
		strlen((char *)pucTaskRunningMsg)
	};
#endif

#pragma pack(1)
	static Dist_Measure_Data xCurrentDistMeasureData = {0};
#pragma pack()
		
	/* Infinite loop */
	for (;;)
	{
		osMessagePeek(xQueueRunTokenDistMeasureHandle, osWaitForever);
		/* add codes here */
		
#ifdef DEBUG
		/* log running message */
		osMessagePut(xQueueLogToPcHandle, (uint32_t)&pxTaskRunningMsg, 0);
		
		#ifdef _DebugTaskDistMeasure
		osDelay(random(5000));
		osMessagePut(xQueueDistMeasureDataHandle, (uint32_t)&xCurrentDistMeasureData, osWaitForever);
		#endif
#endif
		osThreadYield();
	}
}
