#ifndef __EXTRA_H_
#define __EXTRA_H_
/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"
#include "tim.h"
#include "gpio.h"

/* USER CODE BEGIN Includes */
#include "stm32f1xx_hal_tim.h"
#include "math.h"
#include "motor.h"
#include "interface.h"
#include "stdbool.h"
/* USER CODE END Includes */

/*define------------------------------------------------------------------*/
#define TRUE 1
#define FALSE 0


/*function------------------------------------------------------------------*/
void ini_EchoTIM(void);
void Trig_Start(void);
void duoji_rotation(int angle);
void execute(uint16_t IO);
bool judge_Ultra_end(uint32_t CNC);



#endif
