#include "vTaskZigbeeHandler.h"
#include "main.h"
#include "freertos.h"
#include "usart.h"
#include "commonFuntion.h"
#include "vTaskDmaGatekeeper.h"
#include "dma.h"

#define min(x, y) (x >= y? y : x)

#ifdef DEBUG
#endif
/* Queue */
extern osMessageQId xQueueLogToPcHandle;
/* Semaphore */
extern osSemaphoreId xBinSemZigbeeRecieveHandle;

unsigned portCHAR pcZigbeeRx[_ZigbeeRxSize] = {0};
tRingBufObject xZigbeeRxRing;

/* vZigbeeHandlerTask function */
void vTaskZigbeeHandler(void const * argument)
{
	unsigned portSHORT usZigbeeRxXferCount;
	unsigned portSHORT usZigbeeRxRingContigFree;
	RingBufInit(&xZigbeeRxRing, (uint8_t *)pcZigbeeRx, _ZigbeeRxSize);
	HAL_UART_Receive_DMA(_ZigbeeUartHandle, (uint8_t *)pcZigbeeRxBuffer, _ZigbeeRxBufferSize);
	#ifdef DEBUG
	Dma_Gatekeeper_Exchange_Data pxZigbeeRxData = {
		NULL,
		0
	};
	unsigned portCHAR pucTaskTaskZigbeeHandlerRunningMsg[] = "\r\nMessage: Zigbee Handler Task works well.\r\n";
	Dma_Gatekeeper_Exchange_Data pxTaskTaskZigbeeHandlerRunningMsg = {
		pucTaskTaskZigbeeHandlerRunningMsg,
		strlen((char *)pucTaskTaskZigbeeHandlerRunningMsg)
	};
	#endif
	
	osSemaphoreWait(xBinSemZigbeeRecieveHandle, 0);
  /* Infinite loop */
  for(;;)
  {
		/* wait for xBinSemZigbeeRecieveHandle */
		osSemaphoreWait(xBinSemZigbeeRecieveHandle, osWaitForever);
//		memcpy(pcZigbeeRx, pcZigbeeRxBuffer, _ZigbeeUartHandle->RxXferCount);
		
		usZigbeeRxXferCount = _ZigbeeUartHandle->RxXferCount;
		usZigbeeRxRingContigFree = RingBufContigFree(&xZigbeeRxRing);
		memcpy(xZigbeeRxRing.pucBuf + xZigbeeRxRing.ulWriteIndex, pcZigbeeRxBuffer, min(usZigbeeRxXferCount, usZigbeeRxRingContigFree));
		if( usZigbeeRxXferCount > usZigbeeRxRingContigFree){
			memcpy(xZigbeeRxRing.pucBuf, pcZigbeeRxBuffer + usZigbeeRxRingContigFree, usZigbeeRxXferCount - usZigbeeRxRingContigFree);
		}
		RingBufAdvanceWrite(&xZigbeeRxRing, usZigbeeRxXferCount);
		
		#ifdef DEBUG
		pxZigbeeRxData.pucData = xZigbeeRxRing.pucBuf + xZigbeeRxRing.ulReadIndex;
		pxZigbeeRxData.usDataSize = RingBufContigUsed(&xZigbeeRxRing);			
//		pxZigbeeRxData.usDataSize = _ZigbeeUartHandle->RxXferCount;
		if( osOK != osMessagePut ( xQueueLogToPcHandle, (uint32_t)&pxZigbeeRxData, 0 ) ){
			Error_Handler();
		}
		RingBufAdvanceRead(&xZigbeeRxRing, pxZigbeeRxData.usDataSize);
		
		if( osOK != osMessagePut ( xQueueLogToPcHandle, (uint32_t)&pxTaskTaskZigbeeHandlerRunningMsg, 0 ) ){
			Error_Handler();
		}
//		if( osEventTimeout != osDelay(900) ){
////			Error_Handler();
//		}
		#endif
		osThreadYield();
  }
}

void vZigbeeUart_RxCpltCallback(){
//	RingBufAdvanceWrite(&xZigbeeRxRing, _ZigbeeUartHandle->RxXferCount);
//	osSemaphoreWait(xBinSemZigbeeRecieveHandle, 0);
	if( osOK != osSemaphoreRelease( xBinSemZigbeeRecieveHandle ) ){
//					Error_Handler();
	}
//	HAL_UART_DMAStop(_ZigbeeUartHandle);
//	HAL_UART_Receive_DMA(_ZigbeeUartHandle, (uint8_t *)pcZigbeeRxBuffer, _ZigbeeRxBufferSize);
}

void vZigbeeUart_IdleCallback(){
	vZigbeeUart_RxCpltCallback();
	HAL_UART_DMAStop(_ZigbeeUartHandle);
	HAL_UART_Receive_DMA(_ZigbeeUartHandle, (uint8_t *)pcZigbeeRxBuffer, _ZigbeeRxBufferSize);
}
