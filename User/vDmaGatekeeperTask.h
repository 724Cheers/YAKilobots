#ifndef __vDmaGatekeeperTask_H
#define __vDmaGatekeeperTask_H

#include "cmsis_os.h"
#include "usart.h"
#include "commonFuntion.h"

typedef enum{
	Dma_Rx = 0x00,
	Dma_Tx = 0x01
}Exchange_Mode;
typedef struct{
	Exchange_Mode	 ExMode;
	portCHAR		*pcMessage;
}Dma_Gatekeeper_Exchange_Data;

void vDmaGatekeeperTask(void const * argument);

#endif /*__ vDmaGatekeeperTask_H */
