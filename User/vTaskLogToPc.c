#include "vTaskLogToPc.h"
#include "freertos.h"
#include "usart.h"
#include "commonFuntion.h"

/* Queue */
extern osMessageQId xQueueLogToPcHandle;
/* Semaphore */
extern osSemaphoreId xBinSemZigbeeRecieveHandle;

/* vTaskLogToPc function */
void vTaskLogToPc(void const * argument)
{
  /* init code for USB_DEVICE */
  MX_USB_DEVICE_Init();

  /* USER CODE BEGIN vTaskLogToPc */
	osEvent eventLogToPc;
  /* Infinite loop */
  for(;;)
  {
		/* 从xQueueLogToPcHandle中Get要发送的日志 */		
		eventLogToPc = osMessageGet	(	xQueueLogToPcHandle, osWaitForever );
		
		/* 向xQueueDmaTxDataHandle中Put要过DMA发?的数据 */
		if( osOK != osMessagePut ( xQueueDmaHandle, (uint32_t)eventLogToPc.value.p, 0 ) ){
//			Error_Handler();
		}
		/* 向xQueueLogToPcHandle中Put任务运行状态 */
		#ifdef DEBUG
//		vLogToPc("Message: Log To Pc task works well.\r\n");
		#endif
		osThreadYield();
  }
  /* USER CODE END vTaskLogToPc */
}
