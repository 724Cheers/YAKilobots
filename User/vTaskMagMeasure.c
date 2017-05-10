#include "vTaskMagMeasure.h"
#include "freertos.h"
#include "usart.h"
#include "commonFuntion.h"

/* Queue */
extern osMessageQId xQueueLogToPcHandle;
/* Semaphore */

/* vTaskMagMeasure function */
void vTaskMagMeasure(void const * argument)
{
  /* Infinite loop */
  for(;;)
  {
		#ifdef DEBUG
		vLogToPc("\r\nMessage: Mag Measure Task works well.\r\n");
		if( osEventTimeout != osDelay(1000) ){
//			Error_Handler();
		}
		#endif
		osThreadYield();
  }
}
