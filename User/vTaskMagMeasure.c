#include "vTaskMagMeasure.h"
#include "cmsis_os.h"
#include "usart.h"
#include "commonFuntion.h"
#include "vTaskDmaGatekeeper.h"

/* Queue */
extern osMessageQId xQueueLogToPcHandle;
/* Semaphore */

/* vTaskMagMeasure function */
void vTaskMagMeasure(void const * argument)
{
#ifdef DEBUG
	const unsigned portCHAR pucTaskMagMeasureRunningMsg[] = "\r\nMessage: Mag Measure Task works well.\r\n";
	Dma_Gatekeeper_Exchange_Data pxTaskMagMeasureRunningMsg = {
		(unsigned char *)pucTaskMagMeasureRunningMsg,
		strlen((char *)pucTaskMagMeasureRunningMsg)
	};
#endif

	/* Infinite loop */
	for (;;)
	{
#ifdef DEBUG
		/* log running message */
		osMessagePut(xQueueLogToPcHandle, (uint32_t)&pxTaskMagMeasureRunningMsg, 0);
		
		#ifdef _DebugTaskMagMeasure
		#endif
		if (osEventTimeout != osDelay(random(5000))){
			//			Error_Handler(pcTaskGetTaskName(NULL));
		}
#endif
		osThreadYield();
	}
}
