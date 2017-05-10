#ifndef __vTaskZigbeeHandler_H
#define __vTaskZigbeeHandler_H

#include "utils/ringbuf.h"

//#define _DebugZigbeeHandler

extern tRingBufObject xZigbeeRxRing;

void vTaskZigbeeHandler(void const * argument);
void vZigbeeUart_IdleCallback(void);
void vZigbeeUart_RxCpltCallback(void);

#endif /*__vTaskZigbeeHandler_H */
