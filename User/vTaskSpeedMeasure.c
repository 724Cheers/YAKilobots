#include "vTaskSpeedMeasure.h"
#include "cmsis_os.h"
#include "commonFuntion.h"
#include "vTaskDmaGatekeeper.h"

#include "tim.h"
#include "motor.h"

/* Queue */
extern osMessageQId xQueueLogToPcHandle;
extern osMessageQId xQueueSpeedMeasureDataHandle;	/* save data */
extern osMessageQId xQueueRunTokenSpeedMeasureHandle;	/* execute token */
/* Semaphore */

void vTaskSpeedMeasure(void const * argument)
{
#ifdef DEBUG
	const unsigned portCHAR pucTaskRunningMsg[] = "\r\nMessage: Speed Measure Task works well.\r\n";
	Dma_Gatekeeper_Exchange_Data pxTaskRunningMsg = {
		(unsigned char *)pucTaskRunningMsg,
		strlen((char *)pucTaskRunningMsg)
	};
#endif
	
#pragma pack(1)
	static Speed_Measure_Data xCurrentSpeedMeasureData = {0};
#pragma pack()
		
	/* Infinite loop */
	for (;;)
	{
		osMessagePeek(xQueueRunTokenSpeedMeasureHandle, osWaitForever);
		/* --------------------------- */
		/* add codes here */
			/*****²âËÙÄ£¿é²âÊÔ*******/  //ÂÖ°ë¾¶3.2cm    ²âÊÔ×î¸ßËÙ¶ÈÊÇ12cm/s 2016/4/21
		HAL_TIM_Base_Start_IT(&htim8);
		HAL_TIM_IC_Start_IT(&htim8, TIM_CHANNEL_3);
		HAL_TIM_IC_Start_IT(&htim8, TIM_CHANNEL_4);
		CarStop();
		HAL_Delay(50);
		TIM8->CCR1 = 5200;        //right_speed_duty 
		TIM8->CCR2 = 5000;        //left_speed_duty   
		HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_1);
		HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_2);	
		CarGo();
		/* --------------------------- */

#ifdef DEBUG
		/* log running message */
		osMessagePut(xQueueLogToPcHandle, (uint32_t)&pxTaskRunningMsg, 0);		
		
		#ifdef _DebugTaskSpeedMeasure
		osDelay(random(5000));
		osMessagePut(xQueueSpeedMeasureDataHandle, (uint32_t)&xCurrentSpeedMeasureData, osWaitForever);
		#endif		
#endif
		osThreadYield();
	}
}
