#include "FreeRTOS.h"
#include "task.h"
#include "stm32f10x.h"
#include "Port_Manager.h"
#include "HM10.h"
#include <stdio.h>
#include <string.h>
#include "ustime.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
void vApplicationStackOverflowHook(xTaskHandle *pxTask, char *pcTaskName)
{
	while(1);
}

int main(void)
{
	SystemInit();
	Port_Config();
	USART1_Init();
 
	// Create LED blink task
	xTaskCreate(Task_Led,"LED_PB12",128, NULL, 1, NULL);
	xTaskCreate(Task_HM10,"HM10_Test",128, NULL, 1, NULL);
	xTaskCreate(Task_GetString,"Uart _GetString",128, NULL, 1, NULL);
	// Start RTOS scheduler
	vTaskStartScheduler();
	return 0;
}

#ifdef USE_FULL_ASSERT
/**
   * @brief Reports the name of the source file and the source line number
   * where the assert_param error has occurred.
   * @param file: pointer to the source file name
   * @param line: assert_param error line source number
   * @retval None
   */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
    ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */

}
#endif

/**
  * @}
  */
/**
  * @}
*/
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
