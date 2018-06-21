#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "Port_Manager.h"

extern uint8_t Led_Stt;

void Port_Config (void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	 /* GPIOB Periph clock enable */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
  /* Configure PD0 and PD2 in output pushpull mode */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 ;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
}

void Task_Led(void * pvParameters)
{
	for(;;)
	{
    if(Led_Stt == ON){
			GPIO_ResetBits(GPIOB,GPIO_Pin_12);
			Led_Stt = 2;
		}
		else if(Led_Stt == OFF){
			GPIO_SetBits(GPIOB,GPIO_Pin_12);
			Led_Stt = 2;
		}
	}
}
