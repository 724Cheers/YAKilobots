#include "stm32f1xx_hal.h"
#include "tim.h"
#include "gpio.h"

/* USER CODE BEGIN Includes */
#include "stm32f1xx_hal_tim.h"
#include "math.h"
#include "motor.h"
#include "interface.h"
#include "stdbool.h"
#include "EXTRA.h"

/* USER CODE END Includes */

/*UINT32_T variable------------------------------------------------------------------*/
uint32_t duoji_angle[4]={120,150,180,213};
uint32_t Ultrasound_count[6]={0,1,2,3,2,1};   //位置状态
uint32_t count=0;                             //位置状态级别
uint32_t count_1s=0;
uint32_t count_t=0;                           //time[]下标
uint32_t count_flag=0;
uint32_t time[1000];
uint32_t scope=0;
uint32_t count_time = 0;

/*USER variable------------------------------------------------------------------*/
int angle_count=0;
bool Next_Step = 0;
bool Rising_edge[4] = {TRUE,TRUE,TRUE,TRUE};
bool Trig_Flag=TRUE;
bool douji_complet=0;
int count_exti_rise=0;
int count_exti_down=0;
bool stop=FALSE;

/*function------------------------------------------------------------------*/
void ini_EchoTIM(void);
void Trig_Start(void);
void duoji_rotation(int angle);
void execute(uint16_t IO);

bool Extra_judge(bool temp_R,TIM_HandleTypeDef *htim,int flag);
bool judge_Ultra_end(uint32_t CNC);


/*function------------------------------------------------------------------*/
void ini_EchoTIM(void);
void Trig_Start(void);
void duoji_rotation(int angle);
void execute(uint16_t IO);
bool judge_Ultra_end(uint32_t CNC);


bool Extra_judge(bool temp_R,TIM_HandleTypeDef *htim,int flag)
{
	if(temp_R==TRUE)
	{
		HAL_TIM_Base_Start(htim);
		temp_R = FALSE;
		count_exti_rise++;
		if(stop==TRUE){temp_R=FALSE;}			
		return temp_R;
	}
	else 
	{
		HAL_TIM_Base_Stop(htim);
		time[count_t+flag] = __HAL_TIM_GET_COUNTER(htim); 
		temp_R=TRUE;
		scope++;
		count_exti_down++;		
		if(stop==TRUE){temp_R=FALSE;}
		return temp_R;
	}
	
}


void ini_EchoTIM()
{
	MX_TIM2_Init();
	MX_TIM3_Init();
  MX_TIM4_Init();
	MX_TIM5_Init();
	MX_TIM6_Init();
  MX_TIM7_Init();
}


void Trig_Start()
{
	uint16_t i;  //variable for delay to trig
	Trig_HIGH;
	for (i = 0; i<150; i++){}
	Trig_LOW ;
	for (i = 0; i<150; i++){}

		
	//HAL_TIM_Base_Start_IT(&htim5);	
}

void duoji_rotation(int angle)
{	
	HAL_TIM_Base_Start(&htim5);
	HAL_TIM_PWM_Start(&htim5, TIM_CHANNEL_4);
	TIM5->CCR4=duoji_angle[angle];
	HAL_Delay(500);
  Next_Step=FALSE;
}

bool judge_Ultra_end(uint32_t CNC)
{
	if(CNC>4){return TRUE;}
	else return FALSE;
}


void execute(uint16_t IO)
{   	   
		switch (IO)
		{
		case GPIO_PIN_11:Rising_edge[0]=Extra_judge(Rising_edge[0],&htim3,0);break;
		case GPIO_PIN_12:Rising_edge[1]=Extra_judge(Rising_edge[1],&htim4,1); break;
		case GPIO_PIN_13:Rising_edge[2]=Extra_judge(Rising_edge[2],&htim6,2); break;
		case GPIO_PIN_14:Rising_edge[3]=Extra_judge(Rising_edge[3],&htim7,3); break;
		default:break;		
    }	  			 			 
}

