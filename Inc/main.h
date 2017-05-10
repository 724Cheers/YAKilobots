#ifndef __main_H
#define __main_H

#include "cmsis_os.h"
//#include "usart.h"
#include "commonFuntion.h"

#ifndef DEBUG
#define DEBUG
#endif

#define Gv Seed

extern UART_HandleTypeDef huart1;

#define _ZigbeeUartHandle (&huart1)
#define _ZigbeeRxBufferSize 100
#define _ZigbeeRxSize (_ZigbeeRxBufferSize * 3)
//#define _ZigbeeTxBufferSize 100
#define pcZigbeeRxBuffer pcUart1RxBuffer

#define _LogUartHandle _ZigbeeUartHandle

#define ENTER_CRITICAL_CODE 0
#define EXIT_CRITICAL_CODE 0

void consoleLog(portCHAR *msg);

#endif /*__main_H */
