#include "HM10.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include <string.h>
#define RX_BUF_SIZE 20
volatile char RX_FLAG_END_LINE = 0;
volatile char RXi;
char RXc;
char RX_BUF[RX_BUF_SIZE];
char  Str[RX_BUF_SIZE];
uint8_t Led_Stt = 0;

void USART1_Init(void)
{
    /* USART configuration structure for USART1 */
    USART_InitTypeDef USART_InitStructure;
    /* Bit configuration structure for GPIOA PIN9 and PIN10 */
    GPIO_InitTypeDef GPIO_InitStructure;
     
    /* Enalbe clock for USART1, AFIO and GPIOA */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_AFIO | 
                           RCC_APB2Periph_GPIOA, ENABLE);
                            
    /* GPIOA PIN9 alternative function Tx */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    /* GPIOA PIN9 alternative function Rx */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
 
    /* Enable USART1 */
    USART_Cmd(USART1, ENABLE);  
    /* Baud rate 9600, 8-bit data, One stop bit
     * No parity, Do both Rx and Tx, No HW flow control
     */
    USART_InitStructure.USART_BaudRate = 9600;   
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;  
    USART_InitStructure.USART_StopBits = USART_StopBits_1;   
    USART_InitStructure.USART_Parity = USART_Parity_No ;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    /* Configure USART1 */
    USART_Init(USART1, &USART_InitStructure);
    /* Enable RXNE interrupt */
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
    /* Enable USART1 global interrupt */
    NVIC_EnableIRQ(USART1_IRQn);
}

void clear_RXBuffer(void) {
	for (RXi=0; RXi<RX_BUF_SIZE; RXi++)
		RX_BUF[RXi] = '\0';
	RXi = 0;
}
 /**********************************************************
 * USART1 interrupt request handler: 
 *********************************************************/
void USART1_IRQHandler(void)
{
  if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{
    	RXc = USART_ReceiveData(USART1);
    	RX_BUF[RXi++] = RXc;
			if ( RXc == '\r' || RXc == '\n')
			RX_FLAG_END_LINE = 1;
	}
}

void USARTx_SendChar(USART_TypeDef* USARTx, uint8_t Data)
{
	while(USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET);
	USART_SendData(USARTx,Data);
}

void USARTx_SendString(USART_TypeDef* USARTx, uint8_t *Str)
{
	while(*Str)
	{
		USARTx_SendChar(USARTx,*Str); 
		Str++;
	}
}

void Task_GetString(void *pvParameters) {
	for(;;)
	{
		if (RX_FLAG_END_LINE == 1) 
    {       
			strcpy(Str,RX_BUF);
			RX_FLAG_END_LINE = 0;
			clear_RXBuffer();
//			memset(RX_BUF,'\0',sizeof(RX_BUF));	//Clear Array RX_BUF
    }    
	}		
}

void Task_HM10(void * pvParameters)
{
	uint8_t Str_ON[20]= "Led ON\n";
	uint8_t Str_OFF[20]= "Led OFF\n";
	for(;;)
	{
    if(strcmp(Str,"Led:1\n")== 0){
			USARTx_SendString(USART1,Str_ON);
			Led_Stt = ON;
			memset(Str,'\0',sizeof(Str));	//Clear Array Str
			}
		else if(strcmp(Str,"Led:0\n")==0){
			USARTx_SendString(USART1,Str_OFF);
			Led_Stt = OFF;
			memset(Str,'\0',sizeof(Str));	//Clear Array Str
			}		
	}
}


