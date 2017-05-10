#include "vTaskMagMeasure.h"
#include "cmsis_os.h"
#include "usart.h"
#include "commonFuntion.h"
#include "vTaskDmaGatekeeper.h"

/* Queue */
extern osMessageQId xQueueLogToPcHandle;
extern osMessageQId xQueueMagMeasureDataHandle;	/* save data */
extern osMessageQId xQueueRunTokenMagMeasureHandle;	/* execute token */
/* Semaphore */

void vTaskMagMeasure(void const * argument)
{
#ifdef DEBUG
	const unsigned portCHAR pucTaskRunningMsg[] = "\r\nMessage: Mag Measure Task works well.\r\n";
	Dma_Gatekeeper_Exchange_Data pxTaskRunningMsg = {
		(unsigned char *)pucTaskRunningMsg,
		strlen((char *)pucTaskRunningMsg)
	};
#endif
	
#pragma pack(1)
	static Mag_Measure_Data xCurrentMagMeasureData = {0};
#pragma pack()
	/* Infinite loop */
	for (;;)
	{
		osMessagePeek(xQueueRunTokenMagMeasureHandle, osWaitForever);
		/* ----------------------------------------------------------------------------- */
		/* add codes here */
		
		/* ----------------------------------------------------------------------------- */
#ifdef DEBUG
		/* log running message */
		osMessagePut(xQueueLogToPcHandle, (uint32_t)&pxTaskRunningMsg, 0);	
		
		#ifdef _DebugTaskMagMeasure
		osDelay(random(5000));
		osMessagePut(xQueueMagMeasureDataHandle, (uint32_t)&xCurrentMagMeasureData, osWaitForever);
		#endif
#endif
		osThreadYield();
	}
}
