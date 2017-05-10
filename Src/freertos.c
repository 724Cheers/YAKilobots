/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  *
  * COPYRIGHT(c) 2016 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"

/* USER CODE BEGIN Includes */     
#include "usart.h"
#include "vDmaGatekeeperTask.h"
#include "commonFuntion.h"
/* USER CODE END Includes */

/* Variables -----------------------------------------------------------------*/
osThreadId LogToPcHandle;
osThreadId ZigbeeHandlerHandle;
osThreadId MagMeasureHandle;
osThreadId DmaGatekeeperHandle;
osMessageQId xQueueLogToPcHandle;
osMessageQId xQueueZigbeeRecieveHandle;
osMessageQId xQueueDmaHandle;
osMessageQId xQueueTim8UpIrqHandle;
osTimerId xTimerLogToPcHandle;
osTimerId xTimerUltrasonicFrontHandle;
osTimerId xTimerUltrasonicLeftHandle;
osTimerId xTimerUltrasonicBackHandle;
osTimerId xTimerUltrasonicRightHandle;
osTimerId xTimerUltrasonicHandle;
osSemaphoreId xBinSemDmaIsOkHandle;

/* USER CODE BEGIN Variables */

/* USER CODE END Variables */

/* Function prototypes -------------------------------------------------------*/
void vLogToPcTask(void const * argument);
void vZigbeeHandlerTask(void const * argument);
void vMagMeasureTask(void const * argument);
extern void vDmaGatekeeperTask(void const * argument);
void vTimerLogToPcCallback(void const * argument);
void vTimerUltrasonicFrontCallback(void const * argument);
void vTimerUltrasonicLeftCallback(void const * argument);
void vTimerUltrasonicBackCallback(void const * argument);
void vTimerUltrasonicRightCallback(void const * argument);
void vTimerUltrasonicCallback(void const * argument);

extern void MX_USB_DEVICE_Init(void);
void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* USER CODE BEGIN FunctionPrototypes */
/* USER CODE END FunctionPrototypes */

/* Hook prototypes */

/* Init FreeRTOS */

void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */
       
  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* Create the semaphores(s) */
  /* definition and creation of xBinSemDmaIsOk */
  osSemaphoreDef(xBinSemDmaIsOk);
  xBinSemDmaIsOkHandle = osSemaphoreCreate(osSemaphore(xBinSemDmaIsOk), 1);

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* Create the timer(s) */
  /* definition and creation of xTimerLogToPc */
  osTimerDef(xTimerLogToPc, vTimerLogToPcCallback);
  xTimerLogToPcHandle = osTimerCreate(osTimer(xTimerLogToPc), osTimerOnce, NULL);

  /* definition and creation of xTimerUltrasonicFront */
  osTimerDef(xTimerUltrasonicFront, vTimerUltrasonicFrontCallback);
  xTimerUltrasonicFrontHandle = osTimerCreate(osTimer(xTimerUltrasonicFront), osTimerOnce, NULL);

  /* definition and creation of xTimerUltrasonicLeft */
  osTimerDef(xTimerUltrasonicLeft, vTimerUltrasonicLeftCallback);
  xTimerUltrasonicLeftHandle = osTimerCreate(osTimer(xTimerUltrasonicLeft), osTimerOnce, NULL);

  /* definition and creation of xTimerUltrasonicBack */
  osTimerDef(xTimerUltrasonicBack, vTimerUltrasonicBackCallback);
  xTimerUltrasonicBackHandle = osTimerCreate(osTimer(xTimerUltrasonicBack), osTimerOnce, NULL);

  /* definition and creation of xTimerUltrasonicRight */
  osTimerDef(xTimerUltrasonicRight, vTimerUltrasonicRightCallback);
  xTimerUltrasonicRightHandle = osTimerCreate(osTimer(xTimerUltrasonicRight), osTimerOnce, NULL);

  /* definition and creation of xTimerUltrasonic */
  osTimerDef(xTimerUltrasonic, vTimerUltrasonicCallback);
  xTimerUltrasonicHandle = osTimerCreate(osTimer(xTimerUltrasonic), osTimerOnce, NULL);

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
	osTimerStart	(	xTimerLogToPcHandle, 1000 );
  /* USER CODE END RTOS_TIMERS */

  /* Create the thread(s) */
  /* definition and creation of LogToPc */
  osThreadDef(LogToPc, vLogToPcTask, osPriorityIdle, 0, 64);
  LogToPcHandle = osThreadCreate(osThread(LogToPc), NULL);

  /* definition and creation of ZigbeeHandler */
  osThreadDef(ZigbeeHandler, vZigbeeHandlerTask, osPriorityLow, 0, 128);
  ZigbeeHandlerHandle = osThreadCreate(osThread(ZigbeeHandler), NULL);

  /* definition and creation of MagMeasure */
  osThreadDef(MagMeasure, vMagMeasureTask, osPriorityNormal, 0, 64);
  MagMeasureHandle = osThreadCreate(osThread(MagMeasure), NULL);

  /* definition and creation of DmaGatekeeper */
  osThreadDef(DmaGatekeeper, vDmaGatekeeperTask, osPriorityIdle, 0, 64);
  DmaGatekeeperHandle = osThreadCreate(osThread(DmaGatekeeper), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* Create the queue(s) */
  /* definition and creation of xQueueLogToPc */
  osMessageQDef(xQueueLogToPc, 16, uint32_t);
  xQueueLogToPcHandle = osMessageCreate(osMessageQ(xQueueLogToPc), NULL);

  /* definition and creation of xQueueZigbeeRecieve */
  osMessageQDef(xQueueZigbeeRecieve, 1, uint32_t);
  xQueueZigbeeRecieveHandle = osMessageCreate(osMessageQ(xQueueZigbeeRecieve), NULL);

  /* definition and creation of xQueueDma */
  osMessageQDef(xQueueDma, 16, uint32_t);
  xQueueDmaHandle = osMessageCreate(osMessageQ(xQueueDma), NULL);

  /* definition and creation of xQueueTim8UpIrq */
  osMessageQDef(xQueueTim8UpIrq, 16, uint16_t);
  xQueueTim8UpIrqHandle = osMessageCreate(osMessageQ(xQueueTim8UpIrq), NULL);

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */
}

/* vLogToPcTask function */
void vLogToPcTask(void const * argument)
{
  /* init code for USB_DEVICE */
  MX_USB_DEVICE_Init();

  /* USER CODE BEGIN vLogToPcTask */
  /* Infinite loop */
  for(;;)
  {
		/* 从xQueueLogToPcHandle中Get�?要发送的日志 */		
		osEvent eventLogToPc;
		eventLogToPc = osMessageGet	(	xQueueLogToPcHandle, osWaitForever );
		
		/* 向xQueueDmaTxDataHandle中Put�?要�?�过DMA发�?�的数据 */
		if( osOK != osMessagePut ( xQueueDmaHandle, (uint32_t)eventLogToPc.value.p, osWaitForever ) ){
//			Error_Handler();
		}
		/* 向xQueueLogToPcHandle中Put任务运行状�?? */
		#ifdef DEBUG
		
//		Dma_Gatekeeper_Exchange_Data ExData ={
//			Dma_Tx,
//			"Message: Log To Pc task works well.\r\n"
//		};
//		osMessagePut ( xQueueLogToPcHandle, (uint32_t)&ExData, osWaitForever );
//		osDelay(1000);
		#endif
//		osThreadYield();
  }
  /* USER CODE END vLogToPcTask */
}

/* vZigbeeHandlerTask function */
void vZigbeeHandlerTask(void const * argument)
{
  /* USER CODE BEGIN vZigbeeHandlerTask */
  /* Infinite loop */
  for(;;)
  {
//		if(HAL_UART_Receive_DMA(&huart1, (uint8_t *)uart1RxBuffer, RX_BUFFER_SIZE) == HAL_TIMEOUT)
//		{
//			consoleLog("Message: Zigbee Rx timeout.\r\n");
////			Error_Handler();
//		}
//		consoleLog("Message: Zigbee Handler Task works well.\r\n");
		#ifdef DEBUG
		Dma_Gatekeeper_Exchange_Data ZigbeeHandlerExData ={
			Dma_Tx,
			"Message: Zigbee Handler Task works well.\r\n"
		};
		if( osOK != osMessagePut ( xQueueLogToPcHandle, (uint32_t)&ZigbeeHandlerExData, osWaitForever ) ){
			Error_Handler();
		}
		if( osEventTimeout != osDelay(1000) ){
//			Error_Handler();
		}
		#endif
  }
  /* USER CODE END vZigbeeHandlerTask */
}

/* vMagMeasureTask function */
void vMagMeasureTask(void const * argument)
{
  /* USER CODE BEGIN vMagMeasureTask */
  /* Infinite loop */
  for(;;)
  {
		#ifdef DEBUG
		Dma_Gatekeeper_Exchange_Data ExData ={
			Dma_Tx,
			"Message: Mag Measure Task works well.\r\n"
		};
		if( osOK != osMessagePut ( xQueueLogToPcHandle, (uint32_t)&ExData, osWaitForever ) ){
//			Error_Handler();
		}
		if( osEventTimeout != osDelay(1000) ){
//			Error_Handler();
		}
		#endif
  }
  /* USER CODE END vMagMeasureTask */
}

/* vTimerLogToPcCallback function */
void vTimerLogToPcCallback(void const * argument)
{
  /* USER CODE BEGIN vTimerLogToPcCallback */
	#ifdef DEBUG
//	Dma_Gatekeeper_Exchange_Data ExData ={
//		Dma_Tx,
//		"Message: Timer log to Pc callback works well.\r\n"
//	};
//	osMessagePut ( xQueueLogToPcHandle, (uint32_t)&ExData, 0 );

//	osTimerStart	(	xTimerLogToPcHandle, 1000 );
//	
//	BaseType_t xHigherPriorityTaskWoken = pdFALSE;
//	if (xQueueSendFromISR(xQueueLogToPcHandle, &ExData, &xHigherPriorityTaskWoken) != pdTRUE) {
////		return osErrorOS;
//	}

//	/* xTimerStartFromISR() or xTimerResetFromISR() could be called here
//	as both cause the timer to re-calculate its expiry time.
//	xHigherPriorityTaskWoken was initialised to pdFALSE when it was
//	declared (in this function). */
//	if( xTimerResetFromISR( xTimerLogToPcHandle,
//													&xHigherPriorityTaskWoken ) != pdPASS )
//	{
//			/* The reset command was not executed successfully.  Take appropriate
//			action here. */
//	}

//	/* Perform the rest of the key processing here. */

	#endif
  /* USER CODE END vTimerLogToPcCallback */
}

/* vTimerUltrasonicFrontCallback function */
void vTimerUltrasonicFrontCallback(void const * argument)
{
  /* USER CODE BEGIN vTimerUltrasonicFrontCallback */
  
  /* USER CODE END vTimerUltrasonicFrontCallback */
}

/* vTimerUltrasonicLeftCallback function */
void vTimerUltrasonicLeftCallback(void const * argument)
{
  /* USER CODE BEGIN vTimerUltrasonicLeftCallback */
  
  /* USER CODE END vTimerUltrasonicLeftCallback */
}

/* vTimerUltrasonicBackCallback function */
void vTimerUltrasonicBackCallback(void const * argument)
{
  /* USER CODE BEGIN vTimerUltrasonicBackCallback */
  
  /* USER CODE END vTimerUltrasonicBackCallback */
}

/* vTimerUltrasonicRightCallback function */
void vTimerUltrasonicRightCallback(void const * argument)
{
  /* USER CODE BEGIN vTimerUltrasonicRightCallback */
  
  /* USER CODE END vTimerUltrasonicRightCallback */
}

/* vTimerUltrasonicCallback function */
void vTimerUltrasonicCallback(void const * argument)
{
  /* USER CODE BEGIN vTimerUltrasonicCallback */
  
  /* USER CODE END vTimerUltrasonicCallback */
}

/* USER CODE BEGIN Application */

/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
