#ifndef __HM10_H
#define __HM10_H
#define ON 1
#define OFF	0

void USART1_Init(void);
void clear_RXBuffer(void); 
void Task_HM10(void * pvParameters);
void Task_GetString(void *ptr);
#endif
