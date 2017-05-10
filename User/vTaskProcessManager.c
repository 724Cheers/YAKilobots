#include "vTaskProcessManager.h"
#include "vTaskDmaGatekeeper.h"
#include "vTaskPppHandler.h"
#include "vTaskMagMeasure.h"
#include "vTaskSpeedMeasure.h"
#include "vTaskDistMeasure.h"
#include "vTaskMotorControl.h"
#include "commonFuntion.h"

typedef struct{
	double dDirection;
}Mag_Measure_Data;

/* Queue */
/* data */
extern osMessageQId xQueueLogToPcHandle;
extern osMessageQId xQueueSendComDataHandle;
extern osMessageQId xQueueReceivedComDataHandle;	/* vTaskPppHandler grenerate it */
extern osMessageQId xQueueMagMeasureDataHandle;
extern osMessageQId xQueueDistMeasureDataHandle;
extern osMessageQId xQueueSpeedMeasureDataHandle;
extern osMessageQId xQueueMotorControlDataHandle;

/* run token */
extern osMessageQId xQueueRunTokenMotorControlHandle;
extern osMessageQId xQueueRunTokenDistMeasureHandle;
extern osMessageQId xQueueRunTokenSpeedMeasureHandle;
extern osMessageQId xQueueRunTokenMagMeasureHandle;

/* Semaphore */

void vTaskProcessManager(void const * argument)
{
#ifdef DEBUG
	static unsigned portCHAR pucTaskRunningMsg[] = "\r\nMessage: Process Manager Task works well.\r\n";
	Dma_Gatekeeper_Exchange_Data pxTaskRunningMsg = {
		pucTaskRunningMsg,
		strlen((char *)pucTaskRunningMsg)
	};
	const Error_Message xErrMsgFatalInternal = {
		Fatal_Internal,
		pcTaskGetTaskName(NULL)
	};
	const Error_Message xErrMsgNonFatalInternal = {
		NonFatal_Internal,
		pcTaskGetTaskName(NULL)
	};
#endif


	volatile static osEvent xEventMotorControlData;
	volatile static osEvent xEventDistMeasureData;
	volatile static osEvent xEventSpeedMeasureData;
	volatile static osEvent xEventMagMeasureData;
#pragma pack(1)
	volatile static Motor_Control_Data xCurrentMotorControlData = { 0 };
	volatile static Dist_Measure_Data xCurrentDistMeasureData = { 0 };
	volatile static Speed_Measure_Data xCurrentSpeedMeasureData = { 0 };
	volatile static Mag_Measure_Data xCurrentMagMeasureData = { 0 };
	volatile static Com_Data xOneSendData = {
		{ 0, 0, 0 },
		Seed,
		0,
		0,
		0
	};
	osEvent eventReceivedComData;
	static Com_Data pxReceivedComData[_ComNum] = { 0 };
#pragma pack()


	/* Infinite loop */
	for (;;)
	{
		/* ---------------------------------------------- */
		/* 放出令牌,开启磁力计任务 */
		if (osOK != osMessagePut(xQueueRunTokenMagMeasureHandle, NULL, 500)){
			Error_Handler(&xErrMsgFatalInternal);
		}
		/* 等待数据 */
		xEventMagMeasureData = osMessageGet(xQueueMagMeasureDataHandle, osWaitForever);
		/* 清空令牌,停止磁力计任务 */
		xQueueReset(xQueueRunTokenMagMeasureHandle);
		/* 拷贝数据 */
		memcpy((void *)&xCurrentMagMeasureData, xEventMagMeasureData.value.p, sizeof(Mag_Measure_Data));
		/* ---------------------------------------------- */

		/* ---------------------------------------------- */
		/* 放出令牌,开启超声波任务 */
		if (osOK != osMessagePut(xQueueRunTokenDistMeasureHandle, NULL, 500)){
			Error_Handler(&xErrMsgFatalInternal);
		}
		/* 等待数据 */
		xEventDistMeasureData = osMessageGet(xQueueDistMeasureDataHandle, osWaitForever);
		/* 清空令牌,停止超声波任务 */
		xQueueReset(xQueueRunTokenDistMeasureHandle);
		/* 拷贝数据 */
		memcpy((void *)&xCurrentDistMeasureData, xEventDistMeasureData.value.p, sizeof(Dist_Measure_Data));
		/* ---------------------------------------------- */

		/* ---------------------------------------------- */
		/* 处理接收到的它机数据 */
		eventReceivedComData = osMessageGet(xQueueReceivedComDataHandle, 1000);	//??osWaitForever 不靠谱，有可能等不到
		if (osEventMessage == eventReceivedComData.status){
			/* 拷贝数据 */
			memcpy(pxReceivedComData, eventReceivedComData.value.p, _ComNum * _ComDataLength);
#ifdef _DebugTaskPppHandler
			osMessageGet(xQueueSendComDataHandle, 0);
			memcpy(&xOneSendData, &(pxReceivedComData[GV1]), _ComDataLength);
			osMessagePut(xQueueSendComDataHandle, (uint32_t)&xOneSendData, 0);
#endif
		}
		/* ---------------------------------------------- */

		/* ---------------------------------------------- */
		/* 控制策略,根据以上收集到的数据判断接下来动作 */
		//add codes
		/* ---------------------------------------------- */

		/* ---------------------------------------------- */
		/* 放出令牌,开启测速任务 */
		if (osOK != osMessagePut(xQueueRunTokenSpeedMeasureHandle, NULL, 500)){
			Error_Handler(&xErrMsgFatalInternal);
		}
		/* 放出令牌,开启电机任务 */
		if (osOK != osMessagePut(xQueueRunTokenMotorControlHandle, NULL, 500)){
			Error_Handler(&xErrMsgFatalInternal);
		}
		/* 等待数据 */
		xEventMotorControlData = osMessageGet(xQueueMotorControlDataHandle, osWaitForever);
		/* 清空令牌,停止电机、测速任务 */
		xQueueReset(xQueueRunTokenMotorControlHandle);
		xQueueReset(xQueueRunTokenSpeedMeasureHandle);
		/* 拷贝数据 */
		memcpy((void *)&xCurrentMotorControlData, xEventMotorControlData.value.p, sizeof(Motor_Control_Data));
		/* ---------------------------------------------- */

#ifdef DEBUG
		/* log running message */
		osMessagePut(xQueueLogToPcHandle, (uint32_t)&pxTaskRunningMsg, 0);

#ifdef _DebugTaskProcessManager
		/* produce one send data */
		xQueueReset(xQueueSendComDataHandle);
		if (osOK != HAL_RTC_GetTime(&hrtc, (RTC_TimeTypeDef *)&xOneSendData.xTimestamp, RTC_FORMAT_BCD)) {
			Error_Handler(&xErrMsgNonFatalInternal);
		}
		xOneSendData.ulOrientation = 0x1020307E;
		xOneSendData.ulVelocity = 0x5060707D;
		xOneSendData.ulAcceleration = 0x90A0B0C0;
		if (osOK != osMessagePut(xQueueSendComDataHandle, (uint32_t)&xOneSendData, 0)){
			Error_Handler(&xErrMsgNonFatalInternal);
		}
#endif
#endif
		osThreadYield();
	}
}
