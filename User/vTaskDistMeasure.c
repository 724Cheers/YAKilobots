#include "vTaskDistMeasure.h"
#include "cmsis_os.h"
#include "commonFuntion.h"
#include "vTaskDmaGatekeeper.h"

/* modified by qqy at 20160507 */
/* USER CODE BEGIN Includes */
#include "gpio.h"
#include "tim.h"
#include "math.h"
#include "motor.h"
#include "interface.h"
#include "stdbool.h"
#include "EXTRA.h"
/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/
#define _WheelRadius ((float)1)
#define PMW_max (uint32_t)(0xffff);
#define TRUE 1
#define FALSE 0


/*UINT32_T variable------------------------------------------------------------------*/
extern uint32_t duoji_angle[4];
extern uint32_t Ultrasound_count[6];   //Î»ÖÃ×´Ì¬
extern uint32_t count;                             //Î»ÖÃ×´Ì¬¼¶±ð
extern uint32_t count_1s;
extern uint32_t count_t;                           //time[]ÏÂ±ê
extern uint32_t count_flag;
extern uint32_t time[1000];
extern uint32_t scope;
extern uint32_t count_time;

/*USER variable------------------------------------------------------------------*/
extern int angle_count;
extern bool Next_Step ;
extern bool Rising_edge[4] ;
extern bool Trig_Flag;
extern bool douji_complet;
bool flag_it=TRUE;

char right_speed_duty;
char left_speed_duty;

float speed_left;
float speed_right;
float ideal_speed=3.2;

 int temp_t=0;

uint32_t count_speed_right = 0;
uint32_t count_speed_left = 0;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/
/* USER CODE END PFP */
/* modified by qqy at 20160507 end */

/* Queue */
extern osMessageQId xQueueLogToPcHandle;
extern osMessageQId xQueueDistMeasureDataHandle;	/* save data */
extern osMessageQId xQueueRunTokenDistMeasureHandle;	/* execute token */
/* Semaphore */

void vTaskDistMeasure(void const * argument)
{
#ifdef DEBUG
	const unsigned portCHAR pucTaskRunningMsg[] = "\r\nMessage: Dist Measure Task works well.\r\n";
	Dma_Gatekeeper_Exchange_Data pxTaskRunningMsg = {
		(unsigned char *)pucTaskRunningMsg,
		strlen((char *)pucTaskRunningMsg)
	};
#endif

#pragma pack(1)
	static Dist_Measure_Data xCurrentDistMeasureData = {0};
#pragma pack()
		
	/* Infinite loop */
	for (;;)
	{
		osMessagePeek(xQueueRunTokenDistMeasureHandle, osWaitForever);
		/* --------------------------- */
		/* add codes here */
		///********³¬Éù²¨¶æ»úÁª¶¯²âÊÔ*********/
		HAL_TIM_PWM_Start(&htim5, TIM_CHANNEL_4);
		TIM5->CCR4 = 120;
		HAL_Delay(500);

		for(temp_t=0;temp_t<100;temp_t++)
		{ 
			 if(Next_Step==TRUE){duoji_rotation(Ultrasound_count[count]);}
				
			 if(Trig_Flag==TRUE)
				{
					Trig_Start();
					flag_it=TRUE;
					Trig_Flag=FALSE;                //¶æ»ú500msÌ«³¤µ¼ÖÂtrigÐÅºÅ·¢²¼³öÈ¥
					ini_EchoTIM();
					HAL_TIM_Base_Start(&htim2);	
					HAL_TIM_Base_Start_IT(&htim2);
				}

		}    
		count_flag = 0;
		HAL_TIM_Base_Stop_IT(&htim2);	
		/* --------------------------- */
		osMessagePut(xQueueDistMeasureDataHandle, (uint32_t)&xCurrentDistMeasureData, osWaitForever);
#ifdef DEBUG
		/* log running message */
		osMessagePut(xQueueLogToPcHandle, (uint32_t)&pxTaskRunningMsg, 0);
		
		#ifdef _DebugTaskDistMeasure
		osDelay(random(5000));
		osMessagePut(xQueueDistMeasureDataHandle, (uint32_t)&xCurrentDistMeasureData, osWaitForever);
		#endif
#endif
		osThreadYield();
	}
}

/******GPIOÖÐ¶Ï******/
/*
EchoÐÅºÅ½ÓÊÕÖÐ¶Ï
PE11 Ç°
PE12 ºó
PE13 ×ó
PE14 ÓÒ
*/
/********************/
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if(flag_it==TRUE)
	{		
	switch (GPIO_Pin)
	{
	case GPIO_PIN_11:execute(GPIO_PIN_11); break;
	case GPIO_PIN_12:execute(GPIO_PIN_12); break;
	case GPIO_PIN_13:execute(GPIO_PIN_13); break;
	case GPIO_PIN_14:execute(GPIO_PIN_14); break;
	default:break;
	}
		Next_Step=judge_Ultra_end(scope);
		
		 if(Next_Step==TRUE)
		{	
			flag_it=FALSE;
			scope=0;		
			count++;
			ini_EchoTIM();
			count_t=count_t+4;
			if(count>7){count=0;}
			Trig_Flag=TRUE;		
		}
  }	
}

void USER_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{      
	     int temp;  //ÓÃÓÚ³õÊ¼»¯Rising_edge
	
	  /*********3ms¶¨Ê±½áÊø³¬Éù²¨ÈÎÎñ*******/
	if(htim->Instance==htim2.Instance)
	{			
		    count_flag++;
		    flag_it=FALSE;
				HAL_TIM_Base_Stop(&htim2);
				HAL_TIM_Base_Stop(&htim3);
				HAL_TIM_Base_Stop(&htim4);
				HAL_TIM_Base_Stop(&htim6);
				HAL_TIM_Base_Stop(&htim7);
		    HAL_TIM_Base_Stop_IT(&htim2);
		    for(temp=0;temp<4;temp++)
        {
					if(Rising_edge[temp]==FALSE)
						time[count_t+temp]=30;
				} 					
		
		    for(temp=0;temp<4;temp++){Rising_edge[temp]=TRUE;}      				
        Trig_Flag=TRUE;
				
		
	}	
	
	/*******²âËÙÄ£¿éµç»úµ÷ËÙ********/
	else if (htim == &htim8)
	{
		count_time++;
		if (count_time >= 100)
		{
			speed_right = ((float)count_speed_right / 20.0)*_WheelRadius;
			speed_left = ((float)count_speed_left / 20.0)*_WheelRadius;
			
//			if(speed_left>ideal_speed)
//			{
//				TIM8->CCMR2-=50;
//			}
//			else if(speed_left<ideal_speed)
//			{
//				TIM8->CCMR2+=50;
//			}
			
			
			if(fabs(speed_left-speed_right)>=0.1)
			{
				if(speed_left>speed_right)
				{TIM8->CCR1+=200;}
				else if(speed_right>speed_left) 
				{TIM8->CCR1-=200;}	
			}
			else 
			{
				if(fabs(speed_left-speed_right)>=0.01)
				{
					if(speed_left>speed_right)
				  {TIM8->CCR1+=50;}
				  else if(speed_right>speed_left) 
				  {TIM8->CCR1-=50;}
				else
				{
					if(fabs(speed_left-speed_right)>=0.001)
					{
						if(speed_left>speed_right)
						{TIM8->CCR1+=10;}
						else if(speed_right>speed_left) 
						{TIM8->CCR1-=10;}
					}
				}
				}
			}            
			count_time = 0;
			count_speed_right = 0;
			count_speed_left = 0;
		}
	}
}

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
	 /*******²¶»ñ²âËÙÄ£¿éÐÅºÅ*******/
	if (htim == &htim8)
	{  
		if(htim->Channel ==HAL_TIM_ACTIVE_CHANNEL_3)
		count_speed_right++;
	  else if(htim->Channel== HAL_TIM_ACTIVE_CHANNEL_4)
		count_speed_left++;
	}
}
