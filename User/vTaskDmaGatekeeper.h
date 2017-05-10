#ifndef __vTaskDmaGatekeeper_H
#define __vTaskDmaGatekeeper_H

#include "freertos.h"

//typedef enum{
//	Dma_Rx = 0x00,
//	Dma_Tx = 0x01
//}Exchange_Mode;
//typedef struct{
//	Exchange_Mode	 ExMode;
//	portCHAR		*pcMessage;
//}Dma_Gatekeeper_Exchange_Data;

typedef struct{
	unsigned portCHAR		*pucData;
	unsigned portSHORT	usDataSize;
}Dma_Gatekeeper_Exchange_Data;


void vTaskDmaGatekeeper(void const * argument);

#endif /*__vTaskDmaGatekeeper_H */
