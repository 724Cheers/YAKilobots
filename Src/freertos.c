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
#include "vTaskDmaGatekeeper.h"
#include "vTaskZigbeeHandler.h"
#include "vTaskMagMeasure.h"
#include "commonFuntion.h"
#include "string.h"
#include "usart.h"
/* USER CODE END Includes */

/* Variables -----------------------------------------------------------------*/
osThreadId LogToPcHandle;
osThreadId ZigbeeHandlerHandle;
osThreadId MagMeasureHandle;
osThreadId DmaGatekeeperHandle;
osThreadId PppHandlerHandle;
osThreadId ProcessManagerHandle;
osThreadId DistMeasureHandle;
osThreadId SpeedMeasureHandle;
osThreadId MotorControlHandle;
osMessageQId xQueueLogToPcHandle;
osMessageQId xQueueZigbeeRecieveHandle;
osMessageQId xQueueDmaHandle;
osMessageQId xQueueTim8UpIrqHandle;
osMessageQId xQueueReceivedComDataHandle;
osMessageQId xQueueMagMeasureDataHandle;
osMessageQId xQueueDistMeasureDataHandle;
osMessageQId xQueueSpeedMeasureDataHandle;
osMessageQId xQueueMotorControlDataHandle;
osMessageQId xQueueSendComDataHandle;
osMessageQId xQueueRunTokenMotorControlHandle;
osMessageQId xQueueRunTokenDistMeasureHandle;
osMessageQId xQueueRunTokenSpeedMeasureHandle;
osMessageQId xQueueRunTokenMagMeasureHandle;
osTimerId xTimerLogToPcHandle;
osTimerId xTimerUltrasonicFrontHandle;
osTimerId xTimerUltrasonicLeftHandle;
osTimerId xTimerUltrasonicBackHandle;
osTimerId xTimerUltrasonicRightHandle;
osTimerId xTimerUltrasonicHandle;
osSemaphoreId xBinSemDmaIsOkHandle;
osSemaphoreId xBinSemZigbeeRecieveHandle;

/* USER CODE BEGIN Variables */
/* USER CODE END Variables */

/* Function prototypes -------------------------------------------------------*/
void vTaskLogToPc(void const * argument);
extern void vTaskZigbeeHandler(void const * argument);
extern void vTaskMagMeasure(void const * argument);
extern void vTaskDmaGatekeeper(void const * argument);
extern void vTaskPppHandler(void const * argument);
extern void vTaskProcessManager(void const * argument);
extern void vTaskDistMeasure(void const * argument);
extern void vTaskSpeedMeasure(void const * argument);
extern void vTaskMotorControl(void const * argument);
void vTimerLogToPcCallback(void const * argument);
void vTimerUltrasonicFrontCallback(void const * argument);
void vTimerUltrasonicLeftCallback(void const * argument);
void vTimerUltrasonicBackCallback(void const * argument);
void vTimerUltrasonicRightCallback(void const * argument);
void vTimerUltrasonicCallback(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* USER CODE BEGIN FunctionPrototypes */
/* USER CODE END FunctionPrototypes */

/* Hook prototypes */
void vApplicationIdleHook(void);
void vApplicationStackOverflowHook(xTaskHandle xTask, signed char *pcTaskName);

/* USER CODE BEGIN 2 */
unsigned long ulIdleCycleCount = 0UL;
void vApplicationIdleHook( void )
{
   /* vApplicationIdleHook() will only be called if configUSE_IDLE_HOOK is set
   to 1 in FreeRTOSConfig.h. It will be called on each iteration of the idle
   task. It is essential that code added to this hook function never attempts
   to block in any way (for example, call xQueueReceive() with a block time
   specified, or call vTaskDelay()). If the application makes use of the
   vTaskDelete() API function (as this demo application does) then it is also
   important that vApplicationIdleHook() is permitted to return to its calling
   function, because it is the responsibility of the idle task to clean up
   memory allocated by the kernel to any task that has since been deleted. */
	ulIdleCycleCount++;
}
/* USER CODE END 2 */

/* USER CODE BEGIN 4 */
void vApplicationStackOverflowHook(xTaskHandle xTask, signed char *pcTaskName)
{
   /* Run time stack overflow checking is performed if
   configCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2. This hook function is
   called if a stack overflow is detected. */
	consoleLog((portCHAR *)pcTaskName);
	consoleLog(" has been overflowed");
}
/* USER CODE END 4 */

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

  /* definition and creation of xBinSemZigbeeRecieve */
  osSemaphoreDef(xBinSemZigbeeRecieve);
  xBinSemZigbeeRecieveHandle = osSemaphoreCreate(osSemaphore(xBinSemZigbeeRecieve), 1);

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
  osThreadDef(LogToPc, vTaskLogToPc, osPriorityIdle, 0, 64);
  LogToPcHandle = osThreadCreate(osThread(LogToPc), NULL);

  /* definition and creation of ZigbeeHandler */
  osThreadDef(ZigbeeHandler, vTaskZigbeeHandler, osPriorityLow, 0, 128);
  ZigbeeHandlerHandle = osThreadCreate(osThread(ZigbeeHandler), NULL);

  /* definition and creation of MagMeasure */
  osThreadDef(MagMeasure, vTaskMagMeasure, osPriorityNormal, 0, 64);
  MagMeasureHandle = osThreadCreate(osThread(MagMeasure), NULL);

  /* definition and creation of DmaGatekeeper */
  osThreadDef(DmaGatekeeper, vTaskDmaGatekeeper, osPriorityIdle, 0, 64);
  DmaGatekeeperHandle = osThreadCreate(osThread(DmaGatekeeper), NULL);

  /* definition and creation of PppHandler */
  osThreadDef(PppHandler, vTaskPppHandler, osPriorityIdle, 0, 64);
  PppHandlerHandle = osThreadCreate(osThread(PppHandler), NULL);

  /* definition and creation of ProcessManager */
  osThreadDef(ProcessManager, vTaskProcessManager, osPriorityRealtime, 0, 256);
  ProcessManagerHandle = osThreadCreate(osThread(ProcessManager), NULL);

  /* definition and creation of DistMeasure */
  osThreadDef(DistMeasure, vTaskDistMeasure, osPriorityAboveNormal, 0, 64);
  DistMeasureHandle = osThreadCreate(osThread(DistMeasure), NULL);

  /* definition and creation of SpeedMeasure */
  osThreadDef(SpeedMeasure, vTaskSpeedMeasure, osPriorityNormal, 0, 64);
  SpeedMeasureHandle = osThreadCreate(osThread(SpeedMeasure), NULL);

  /* definition and creation of MotorControl */
  osThreadDef(MotorControl, vTaskMotorControl, osPriorityHigh, 0, 64);
  MotorControlHandle = osThreadCreate(osThread(MotorControl), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* Create the queue(s) */
  /* definition and creation of xQueueLogToPc */
  osMessageQDef(xQueueLogToPc, 100, uint32_t);
  xQueueLogToPcHandle = osMessageCreate(osMessageQ(xQueueLogToPc), NULL);

  /* definition and creation of xQueueZigbeeRecieve */
  osMessageQDef(xQueueZigbeeRecieve, 1, uint32_t);
  xQueueZigbeeRecieveHandle = osMessageCreate(osMessageQ(xQueueZigbeeRecieve), NULL);

  /* definition and creation of xQueueDma */
  osMessageQDef(xQueueDma, 100, uint32_t);
  xQueueDmaHandle = osMessageCreate(osMessageQ(xQueueDma), NULL);

  /* definition and creation of xQueueTim8UpIrq */
  osMessageQDef(xQueueTim8UpIrq, 16, uint16_t);
  xQueueTim8UpIrqHandle = osMessageCreate(osMessageQ(xQueueTim8UpIrq), NULL);

  /* definition and creation of xQueueReceivedComData */
  osMessageQDef(xQueueReceivedComData, 1, uint32_t);
  xQueueReceivedComDataHandle = osMessageCreate(osMessageQ(xQueueReceivedComData), NULL);

  /* definition and creation of xQueueMagMeasureData */
  osMessageQDef(xQueueMagMeasureData, 1, uint32_t);
  xQueueMagMeasureDataHandle = osMessageCreate(osMessageQ(xQueueMagMeasureData), NULL);

  /* definition and creation of xQueueDistMeasureData */
  osMessageQDef(xQueueDistMeasureData, 1, uint32_t);
  xQueueDistMeasureDataHandle = osMessageCreate(osMessageQ(xQueueDistMeasureData), NULL);

  /* definition and creation of xQueueSpeedMeasureData */
  osMessageQDef(xQueueSpeedMeasureData, 1, uint32_t);
  xQueueSpeedMeasureDataHandle = osMessageCreate(osMessageQ(xQueueSpeedMeasureData), NULL);

  /* definition and creation of xQueueMotorControlData */
  osMessageQDef(xQueueMotorControlData, 1, uint32_t);
  xQueueMotorControlDataHandle = osMessageCreate(osMessageQ(xQueueMotorControlData), NULL);

  /* definition and creation of xQueueSendComData */
  osMessageQDef(xQueueSendComData, 1, uint32_t);
  xQueueSendComDataHandle = osMessageCreate(osMessageQ(xQueueSendComData), NULL);

  /* definition and creation of xQueueRunTokenMotorControl */
  osMessageQDef(xQueueRunTokenMotorControl, 1, uint8_t);
  xQueueRunTokenMotorControlHandle = osMessageCreate(osMessageQ(xQueueRunTokenMotorControl), NULL);

  /* definition and creation of xQueueRunTokenDistMeasure */
  osMessageQDef(xQueueRunTokenDistMeasure, 1, uint8_t);
  xQueueRunTokenDistMeasureHandle = osMessageCreate(osMessageQ(xQueueRunTokenDistMeasure), NULL);

  /* definition and creation of xQueueRunTokenSpeedMeasure */
  osMessageQDef(xQueueRunTokenSpeedMeasure, 1, uint8_t);
  xQueueRunTokenSpeedMeasureHandle = osMessageCreate(osMessageQ(xQueueRunTokenSpeedMeasure), NULL);

  /* definition and creation of xQueueRunTokenMagMeasure */
  osMessageQDef(xQueueRunTokenMagMeasure, 1, uint8_t);
  xQueueRunTokenMagMeasureHandle = osMessageCreate(osMessageQ(xQueueRunTokenMagMeasure), NULL);

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */
}

/* vTaskLogToPc function */
void vTaskLogToPc(void const * argument)
{

  /* USER CODE BEGIN vTaskLogToPc */
	osEvent eventLogToPc;
  /* Infinite loop */
  for(;;)
  {
		/* 从xQueueLogToPcHandle中Get要发送的日志 */		
		eventLogToPc = osMessageGet	(	xQueueLogToPcHandle, osWaitForever );
		if(0 == uxQueueMessagesWaiting(xQueueDmaHandle)){
			/* 向xQueueDmaTxDataHandle中Put要�?�过DMA发�?�的数据 */
			if( osOK != osMessagePut ( xQueueDmaHandle, (uint32_t)eventLogToPc.value.p, 0 ) ){
	//			Error_Handler(pcTaskGetTaskName(NULL));
			}
		}
		osThreadYield();
  }
  /* USER CODE END vTaskLogToPc */
}

/* vTimerLogToPcCallback function */
void vTimerLogToPcCallback(void const * argument)
{
  /* USER CODE BEGIN vTimerLogToPcCallback */
	#ifdef DEBUG
	
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
