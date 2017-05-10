#ifndef __main_H
#define __main_H

#include "cmsis_os.h"
//#include "usart.h"
#include "commonFuntion.h"


#ifndef DEBUG
#define DEBUG
#endif

#define _Gv Seed	/* Gradient Value */
#define _ComNum 4	/* 通信机子的数量 */

extern UART_HandleTypeDef huart1;

#define _ZigbeeUartHandle (&huart1)
#define _ZigbeeRxBufferSize 100
#define _ZigbeeRxSize (_ZigbeeRxBufferSize * 3)
//#define _ZigbeeTxBufferSize 100
#define pcZigbeeRxBuffer pcUart1RxBuffer

#define _LogUartHandle _ZigbeeUartHandle

#define ENTER_CRITICAL_CODE 0
#define EXIT_CRITICAL_CODE 0

#define _PrintfUartHandle _LogUartHandle

#define _Mpu9255I2cHandle (&hi2c1)

void consoleLog(portCHAR *msg);

#endif /*__main_H */
