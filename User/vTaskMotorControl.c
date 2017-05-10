#include "vTaskMotorControl.h"
#include "cmsis_os.h"
#include "commonFuntion.h"
#include "vTaskDmaGatekeeper.h"

/* Queue */
extern osMessageQId xQueueLogToPcHandle;
extern osMessageQId xQueueMotorControlDataHandle;	/* save data */
extern osMessageQId xQueueRunTokenMotorControlHandle;	/* execute token */
/* Semaphore */

void vTaskMotorControl(void const * argument)
{
#ifdef DEBUG
	const unsigned portCHAR pucTaskRunningMsg[] = "\r\nMessage: Motor Control Task works well.\r\n";
	Dma_Gatekeeper_Exchange_Data pxTaskRunningMsg = {
		(unsigned char *)pucTaskRunningMsg,
		strlen((char *)pucTaskRunningMsg)
	};
#endif
	
#pragma pack(1)
	static Motor_Control_Data xCurrentMotorControlData = {0};
#pragma pack()

	/* Infinite loop */
	for (;;)
	{
		osMessagePeek(xQueueRunTokenMotorControlHandle, osWaitForever);
		/* add codes here */
		
#ifdef DEBUG
		/* log running message */
		osMessagePut(xQueueLogToPcHandle, (uint32_t)&pxTaskRunningMsg, 0);
		
		#ifdef _DebugTaskMotorControl
		osDelay(random(5000));
		osMessagePut(xQueueMotorControlDataHandle, (uint32_t)&xCurrentMotorControlData, osWaitForever);
		#endif
#endif
		osThreadYield();
	}
}
