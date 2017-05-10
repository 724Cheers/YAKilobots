#include "vTaskPppHandler.h"
#include "cmsis_os.h"
#include "vTaskZigbeeHandler.h"
#include "main.h"
#include "vTaskDmaGatekeeper.h"
#include "PPP/ahdlc.h"

#define _SendFrameBufferSize (4 + sizeof(Com_Data) * 2)

/* Queue */
extern osMessageQId xQueueSendComFrameHandle;
extern osMessageQId xQueueDmaHandle;
/* Semaphore */

portCHAR cReceivedDataBuffer[_ReceivedDataBufferSize] = {0};
tRingBufObject xReceivedDataBufferRing;

portCHAR cSendFrameBuffer[_SendFrameBufferSize] = {0};

void vTaskPppHandler(void const * argument)
{
	osEvent eventComData;	
	Dma_Gatekeeper_Exchange_Data xSendFrameExData = {
		(unsigned portCHAR *)cSendFrameBuffer,
		0
	};
	ahdlc_init(cReceivedDataBuffer, _ReceivedDataBufferSize, cSendFrameBuffer, _SendFrameBufferSize);
	#pragma pack(1)
	Com_Data xSendDataBuffer = {
		{0,0,0},
		Seed,
		0,
		0,
		0
	};
	for(;;)
	{
		/* sending */
		eventComData = osMessageGet(xQueueSendComFrameHandle, 0);
		if(osEventMessage == eventComData.status){
			memcpy(&xSendDataBuffer, eventComData.value.p, sizeof(Com_Data));			
			xSendFrameExData.usDataSize = ahdlc_tx((portCHAR *)&xSendDataBuffer, sizeof(Com_Data));
		}
		if(xSendFrameExData.usDataSize){
			if( osOK != osMessagePut ( xQueueDmaHandle, (uint32_t)&xSendFrameExData, 0 ) ){
				Error_Handler();
			}
		}
		
		/* receiving */
		if(!RingBufEmpty(&xZigbeeRxRing)){
			ahdlc_rx(RingBufReadOne(&xZigbeeRxRing));
		}
		osThreadYield();
	}
}

void ppp_upcallX(portCHAR *buffer, portSHORT len)
{
	#ifdef DEBUG
	Dma_Gatekeeper_Exchange_Data xReceivedFrameExData = {
		(unsigned portCHAR *)buffer,
		len
	};	
		if( osOK != osMessagePut ( xQueueDmaHandle, (uint32_t)&xReceivedFrameExData, osWaitForever ) ){
		Error_Handler();
	}
	#endif
}
