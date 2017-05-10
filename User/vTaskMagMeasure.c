#include "vTaskMagMeasure.h"
#include "freertos.h"
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
	unsigned portCHAR pucTaskMagMeasureRunningMsg[] = "\r\nMessage: Mag Measure Task works well.\r\n";
	Dma_Gatekeeper_Exchange_Data pxTaskMagMeasureRunningMsg = {
		pucTaskMagMeasureRunningMsg,
		strlen((char *)pucTaskMagMeasureRunningMsg)
	};
	#endif

  /* Infinite loop */
  for(;;)
  {
		#ifdef DEBUG
		if( osOK != osMessagePut ( xQueueLogToPcHandle, (uint32_t)&pxTaskMagMeasureRunningMsg, 0 ) ){
			Error_Handler();
		}
		if( osEventTimeout != osDelay(5000) ){
//			Error_Handler();
		}
		#endif
		osThreadYield();
  }
}
