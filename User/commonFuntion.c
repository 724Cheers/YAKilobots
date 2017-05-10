#include "commonFuntion.h"
#include "vTaskDmaGatekeeper.h"
#include "main.h"
#include "cmsis_os.h"

extern osMessageQId xQueueLogToPcHandle;

#pragma import(__use_no_semihosting)  
void _sys_exit(int x)  { x = x; }
struct __FILE  {
	int handle;
};
FILE __stdout;

#ifdef __GNUC__
/* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
set to 'Yes') calls __io_putchar() */
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */

/**
* @brief  Retargets the C library printf function to the USART.
* @param  None
* @retval None
*/
PUTCHAR_PROTOTYPE
{
	/* Place your implementation of fputc here */
	/* e.g. write a character to the USART1 and Loop until the end of transmission */
	HAL_UART_Transmit(_PrintfUartHandle, (uint8_t *)&ch, 1, 0xFFFF);

	return ch;
}

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
	if (osOK != osMessagePut(xQueueLogToPcHandle, (uint32_t)pxExData, 0)){
		//		Error_Handler(pcTaskGetTaskName(NULL));
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
void Error_Handler_Prototype(const Error_Message *pxErrMsg, unsigned portCHAR* pucFile, unsigned portLONG ucLine)
{
	/* Turn LED2 on */
	//  BSP_LED_On(LED2);
	portCHAR *pcErrType;
//	while (1)
//	{
		/* Error if LED2 is slowly blinking (1 sec. period) */
		//    BSP_LED_Toggle(LED2);
		//		while (!(HAL_UART_STATE_READY == HAL_UART_GetState(_LogUartHandle) ||
		//			HAL_UART_STATE_BUSY_RX == HAL_UART_GetState(_LogUartHandle))){
		//		};
		//		HAL_UART_Transmit_DMA(_LogUartHandle, (uint8_t *)"Error: ", 7);
		//		while (!(HAL_UART_STATE_READY == HAL_UART_GetState(_LogUartHandle) ||
		//			HAL_UART_STATE_BUSY_RX == HAL_UART_GetState(_LogUartHandle))){
		//		};
		//		HAL_UART_Transmit_DMA(_LogUartHandle, (uint8_t *)pcTaskName, strlen(pcTaskName));

		switch ((portLONG)pxErrMsg->xErrType){
		case Fatal_User:
			pcErrType = "Fatal User";
			break;
		case Fatal_Internal:
			pcErrType = "Fatal Internal";
			break;
		case NonFatal_User:
			pcErrType = "NonFatal User";
			break;
		case NonFatal_Internal:
			pcErrType = "NonFatal Internal";
			break;
		default:
			pcErrType = "Unknown";
		}
		//		printf("Error: \"%s\" task generates error!\r\n", pcTaskName);
		printf("\r\nError: [%s] \"%s\" task generates error!(file %s on line %ld)\r\n", pxErrMsg->pcTaskName, pcErrType, pucFile, ucLine);
		while(Fatal_User == pxErrMsg->xErrType || Fatal_Internal == pxErrMsg->xErrType){};
//		HAL_Delay(1000);
//	}
}

//rtc.c
//sscanf(__TIME__, "%2x:%2x:%2x", &(sTime.Hours), &(sTime.Minutes), &(sTime.Seconds));
//sTime.Hours = sTime.Hours % 12;
//sscanf(__DATE__, "%*s %2x 20%2x", &(DateToUpdate.Date), &(DateToUpdate.Year));
