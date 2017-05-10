#include "vTaskPppHandler.h"
#include "cmsis_os.h"
#include "vTaskZigbeeHandler.h"
#include "main.h"
#include "vTaskDmaGatekeeper.h"
#include "PPP/ahdlc.h"
#include "commonFuntion.h"

#define _SendFrameBufferSize (_ZigbeeHeaderLength + 4 + _ComDataLength * 2)

/* Queue */
extern osMessageQId xQueueLogToPcHandle;
extern osMessageQId xQueueSendComDataHandle;	/* vTaskProcessManager grenerate it */
extern osMessageQId xQueueDmaHandle;
extern osMessageQId xQueueReceivedComDataHandle;
/* Semaphore */

/* 未处理接收数据缓存 */
static Com_Data pxReceivedComData[_ComNum] = { 0 };
///* 未处理接收数据环形缓冲区 */
//static tRingBufObject xReceivedDataRing;

void vTaskPppHandler(void const * argument)
{
#ifdef DEBUG
	const unsigned portCHAR pucTaskRunningMsg[] = "\r\nMessage: PPP Handler Task works well.\r\n";
	Dma_Gatekeeper_Exchange_Data pxTaskTaskZigbeeHandlerRunningMsg = {
		(unsigned char *)pucTaskRunningMsg,
		strlen((char *)pucTaskRunningMsg)
	};
#endif

	const Error_Message xErrMsgNonFatalInternal = {
		NonFatal_Internal,
		pcTaskGetTaskName(NULL)
	};
	
	static osEvent eventComData;
	
	/* data received */
	/* 单帧接收数据缓存 */
	static portCHAR pcReceivedComDataBuffer[_ReceivedComDataBufferSize] = { 0 };
	
//	/* 初始化未处理接收数据环形缓冲区 */
//	RingBufInit(&xReceivedDataRing, (unsigned char *)pcReceivedData, _ReceivedDataSize);

	/* data to send */
#pragma pack(1)
	/* 欲发送的数据缓存 */
	static Com_Data xSendDataBuffer = {
		{ 0, 0, 0 },
		_Gv,
		0,
		0,
		0
	};
	/* 欲发送的帧缓存 */
	static portCHAR cSendFrameBuffer[_SendFrameBufferSize] = { 0 };
	/* 添加Zigbee固有Header */
	memcpy(cSendFrameBuffer, &"\xFD\x00\xFF\xFF", _ZigbeeHeaderLength);
	/* 向DMA提交的数据帧 */
	static Dma_Gatekeeper_Exchange_Data xSendFrameExData = {
		(unsigned portCHAR *)cSendFrameBuffer,
		0
	};

	/* 初始化PPP协议的组解帧处理函数 */
	ahdlc_init(pcReceivedComDataBuffer, _ReceivedComDataBufferSize, cSendFrameBuffer + _ZigbeeHeaderLength, _SendFrameBufferSize - _ZigbeeHeaderLength);
	
	for (;;)
	{
		/* sending */
		eventComData = osMessageGet(xQueueSendComDataHandle, 0);/* 不等待,没有新数据就发老数据 */
		/* 判断是否有新数据 */
		if (osEventMessage == eventComData.status){
			memcpy(&xSendDataBuffer, eventComData.value.p, _ComDataLength);
			xSendFrameExData.usDataSize = ahdlc_tx((portCHAR *)&xSendDataBuffer, _ComDataLength);
			
			/* 添加Zigbee的Header中的数据长度	*/
			cSendFrameBuffer[1] = (portCHAR)xSendFrameExData.usDataSize;
			xSendFrameExData.usDataSize += _ZigbeeHeaderLength;
		}
		/* 只要DMA数据帧长度合理，就不停push数据到串口 */
		if (0 < xSendFrameExData.usDataSize && xSendFrameExData.usDataSize <= _ZigbeeFrameMaxLength){
			if (osOK != osMessagePut(xQueueDmaHandle, (uint32_t)&xSendFrameExData, 0)){
//				Error_Handler(pcTaskGetTaskName(NULL));
			}
		}

		/* receiving */
		while (!RingBufEmpty(&xZigbeeRxRing)){
			ahdlc_rx(RingBufReadOne(&xZigbeeRxRing));
		}
		
		#ifdef DEBUG
		/* 向PC推送运行消息 */
		osMessagePut(xQueueLogToPcHandle, (uint32_t)&pxTaskTaskZigbeeHandlerRunningMsg, 0);
		#endif
		/* Sleep for a random time <= _PppSleepRandomTimeMax */
		if (osEventTimeout != osDelay(random(_PppSleepRandomTimeMax))){
			//			Error_Handler(pcTaskGetTaskName(NULL));
		}
		osThreadYield();
	}
}

/* 收到完整一帧数据的回调 */
void ppp_upcallX(portCHAR *buffer, portSHORT len)
{
//	memcpy(xReceivedDataRing.pucBuf + xReceivedDataRing.ulWriteIndex, buffer, len);
//	RingBufAdvanceWrite(&xZigbeeRxRing, len);
	static Com_Data *pxReceivedComDataBuffer = NULL;
	pxReceivedComDataBuffer = (Com_Data *)buffer;
	
	/* 判断数据来源是否合理 */
	if((portCHAR)pxReceivedComDataBuffer->xGv < _ComNum && len <= _ComDataLength){
		
		/* 清空消息，防止修改过程中其他任务访问 */
		xQueueReset(xQueueReceivedComDataHandle);
		
		/* 拷贝消息 */
		memcpy(&(pxReceivedComData[(portCHAR)(pxReceivedComDataBuffer->xGv)]), pxReceivedComDataBuffer, min(len, _ComDataLength));
		
		/* 通知上层应用 */
		if (osOK != osMessagePut(xQueueReceivedComDataHandle, (uint32_t)pxReceivedComData, 0)){
	//			Error_Handler(xErrMsgNonFatalInternal);
		}
	}

#ifdef _DebugTaskPppHandler
	static Dma_Gatekeeper_Exchange_Data xReceivedComDataExData = {
		NULL,
		0
	};
	xReceivedComDataExData.pucData = (unsigned portCHAR *)buffer;
	xReceivedComDataExData.usDataSize = len;
	if (osOK != osMessagePut(xQueueDmaHandle, (uint32_t)&xReceivedComDataExData, osWaitForever)){
//		Error_Handler(pcTaskGetTaskName(NULL));
	}
#endif
}
