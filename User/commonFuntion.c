#include "commonFuntion.h"
#include "vTaskDmaGatekeeper.h"

extern osMessageQId xQueueLogToPcHandle;

/**
  * @brief  Display the current time and date.
  * @param  showtime : pointer to buffer
  * @param  showdate : pointer to buffer
  * @retval None
  */
void RTC_CalendarShow(RTC_HandleTypeDef RtcHandle, portCHAR *showtime, portCHAR *showdate)
{
  RTC_DateTypeDef sdatestructureget;
  RTC_TimeTypeDef stimestructureget;

  /* Get the RTC current Time */
  HAL_RTC_GetTime(&RtcHandle, &stimestructureget, RTC_FORMAT_BIN);
  /* Get the RTC current Date */
  HAL_RTC_GetDate(&RtcHandle, &sdatestructureget, RTC_FORMAT_BIN);
  /* Display time Format : hh:mm:ss */
  sprintf((char *)showtime, "%02d:%02d:%02d", stimestructureget.Hours, stimestructureget.Minutes, stimestructureget.Seconds);
  /* Display date Format : yy-mm-dd */
  sprintf((char *)showdate, "%2d-%02d-%02d", 2000 + sdatestructureget.Year, sdatestructureget.Month, sdatestructureget.Date);
}

void vLogToPc(Dma_Gatekeeper_Exchange_Data *pxExData) {	
	if( osOK != osMessagePut ( xQueueLogToPcHandle, (uint32_t)pxExData, 0 ) ){
		Error_Handler();
	}
}

//void vLogToPcFromISR(portCHAR *pcMsg){
//	portBASE_TYPE taskWoken = pdFALSE;
//	Dma_Gatekeeper_Exchange_Data ExData = {
//			Dma_Tx,
//			pcMsg
//		};
//	ExData.ExMode = Dma_Tx;
//	if (xQueueSendFromISR(xQueueLogToPcHandle, &ExData, &taskWoken) != pdTRUE) {
//	}
//	portEND_SWITCHING_ISR(taskWoken);
//}
void Error_Handler(void)
{
  /* Turn LED2 on */
//  BSP_LED_On(LED2);
  while(1)
  {
    /* Error if LED2 is slowly blinking (1 sec. period) */
//    BSP_LED_Toggle(LED2);
    HAL_Delay(1000);
  }  
}

//rtc.c
//sscanf(__TIME__, "%2x:%2x:%2x", &(sTime.Hours), &(sTime.Minutes), &(sTime.Seconds));
//sTime.Hours = sTime.Hours % 12;
//sscanf(__DATE__, "%*s %2x 20%2x", &(DateToUpdate.Date), &(DateToUpdate.Year));


