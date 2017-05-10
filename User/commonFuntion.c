#include "commonFuntion.h"
#include "vDmaGatekeeperTask.h"

extern osMessageQId xQueueLogToPcHandle;

//void vLogToPc(portCHAR *pcMsg){
//	Dma_Gatekeeper_Exchange_Data ExData = {
//		Dma_Tx,
//		pcMsg
//	};
//	osMessagePut	(	xQueueLogToPcHandle, (uint32_t)&ExData, osWaitForever );
//}
//void vLogToPcFromISR(portCHAR *pcMsg){
//	portBASE_TYPE taskWoken = pdFALSE;
//	Dma_Gatekeeper_Exchange_Data ExData = {
//			Dma_Tx,
//			pcMsg
//		};
//	ExData.ExMode = Dma_Tx;
//	if (xQueueSendFromISR(xQueueLogToPcHandle, &ExData, &taskWoken) != pdTRUE) {
//	}
//	portEND_SWITCHING_ISR(taskWoken);
//}
void Error_Handler(void)
{
  /* Turn LED2 on */
//  BSP_LED_On(LED2);
  while(1)
  {
    /* Error if LED2 is slowly blinking (1 sec. period) */
//    BSP_LED_Toggle(LED2);
    HAL_Delay(1000);
  }  
}
