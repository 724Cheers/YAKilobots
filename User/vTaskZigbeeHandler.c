#include "vTaskZigbeeHandler.h"
#include "freertos.h"
#include "usart.h"
#include "commonFuntion.h"

/* Queue */
extern osMessageQId xQueueLogToPcHandle;
/* Semaphore */
extern osSemaphoreId xBinSemZigbeeRecieveHandle;

/* vZigbeeHandlerTask function */
void vTaskZigbeeHandler(void const * argument)
{
	portCHAR pcZigbeeRx[_RxBufferSize + 1] = {0};
  /* Infinite loop */
  for(;;)
  {
		/* wait for xBinSemZigbeeRecieveHandle */
		osSemaphoreWait(xBinSemZigbeeRecieveHandle, osWaitForever);
		/* clear pcZigbeeRx */
		memset (pcZigbeeRx, '\0', _RxBufferSize + 1);
		/* copy pcZigbeeRxBuffer to pcZigbeeRx */
		memcpy(pcZigbeeRx, pcZigbeeRxBuffer, _ZigbeeUartHandle.RxXferCount);
		/* clear pcZigbeeRxBuffer */
		memset (pcZigbeeRxBuffer, '\0', _RxBufferSize);
		
		#ifdef DEBUG
		vLogToPc( pcZigbeeRx );
		vLogToPc( "\r\nMessage: Zigbee Handler Task works well.\r\n" );
//		if( osEventTimeout != osDelay(900) ){
////			Error_Handler();
//		}
		#endif
		osThreadYield();
  }
}
