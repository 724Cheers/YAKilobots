#include "vTaskDmaGatekeeper.h"
#include "cmsis_os.h"
#include "usart.h"
#include "commonFuntion.h"

extern osMessageQId xQueueDmaHandle;

void vTaskDmaGatekeeper(void const * argument){
	osEvent eventExData;
//	Dma_Gatekeeper_Exchange_Data *pExData;
//	Exchange_Mode ExMode;
	portCHAR *pcExMessage = NULL;
	for( ;; ){
		/* 从xQueueDmaTxDataHandle中Get数据交换的信息 */
		eventExData = osMessageGet (xQueueDmaHandle, osWaitForever);
		
//		pExData = (Dma_Gatekeeper_Exchange_Data *)eventExData.value.p;
//		ExMode = pExData->ExMode;
//		pcExMessage = pExData->pcMessage;
		pcExMessage = eventExData.value.p;
		if(NULL != pcExMessage && 0 != strlen(pcExMessage)){
		/* DMA总线可用 */
		if( HAL_UART_STATE_READY == huart1.State ){
			/* 数据交换模式为Tx */
//			if(Dma_Tx == ExMode){
				if(HAL_UART_Transmit_DMA(&huart1, (uint8_t *)pcExMessage, strlen(pcExMessage) ) != HAL_OK)
				{
					Error_Handler();
				}
//			}
		}
	}
		osThreadYield();
	}
}
