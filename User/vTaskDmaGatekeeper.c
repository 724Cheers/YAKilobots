#include "vTaskDmaGatekeeper.h"
#include "cmsis_os.h"
#include "usart.h"
#include "commonFuntion.h"


extern osMessageQId xQueueDmaHandle;

void vTaskDmaGatekeeper(void const * argument){
	osEvent eventExData;
	Dma_Gatekeeper_Exchange_Data *pxExData = NULL;
	unsigned portCHAR		*pucData = NULL;
	unsigned portSHORT	usDataSize = 0;

	//	Exchange_Mode ExMode;
	for (;;){
		/* DMA总线可用 */
		if (HAL_UART_STATE_READY == HAL_UART_GetState(_LogUartHandle) ||
			HAL_UART_STATE_BUSY_RX == HAL_UART_GetState(_LogUartHandle))
		{
			/* 从xQueueDmaTxDataHandle中Get数据交换的信息 */
			eventExData = osMessageGet(xQueueDmaHandle, osWaitForever);

			pxExData = (Dma_Gatekeeper_Exchange_Data *)eventExData.value.p;
			//		ExMode = pExData->ExMode;
			pucData = pxExData->pucData;
			usDataSize = pxExData->usDataSize;
			if (NULL != pucData && 0 != usDataSize){
				if (HAL_UART_Transmit_DMA(_LogUartHandle, pucData, usDataSize) != HAL_OK)
				{
//					Error_Handler(pcTaskGetTaskName(NULL));
				}
			}
		}
		osThreadYield();
	}
}
