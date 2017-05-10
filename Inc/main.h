#ifndef __main_H
#define __main_H

#include "cmsis_os.h"
#include "usart.h"
#include "commonFuntion.h"

#ifndef DEBUG
#define DEBUG
#endif

#define _ZigbeeUartHandle huart1

void consoleLog(portCHAR *msg);

#endif /*__main_H */
