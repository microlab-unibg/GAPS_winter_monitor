#include "clock.h"
#include "stm32l4xx.h"

const uint32_t CLOCK_SECOND = 1000;

/**
 * @brief  Clock_Init
 * @param  None
 * @retval None
 */
void Clock_Init(void) {
	// FIXME: as long as Cube HAL is initialized this is OK
	// Cube HAL default is one clock each 1 ms
}

/**
 * @brief  Clock_Time
 * @param  None
 * @retval tClockTime
 */
tClockTime Clock_Time(void) {
	return HAL_GetTick();
}

/**
 * @brief  Clock_Wait Wait for a multiple of 1 ms.
 * @param  int i
 * @retval None
 */
void Clock_Wait(uint32_t i) {
	HAL_Delay(i);
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
