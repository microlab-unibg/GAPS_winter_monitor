/**
 ******************************************************************************
 * @file           : IO.c
 * @brief          : Implementation of IO module
 * @author		   : Patrick Locatelli (patrick.locatelli@unibg.it)
 * @version		   : 1.0
 * @date    	   : 13 Feb 2020
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "IO.h"

void IO_ToggleLED(uint8_t led) {
	if (led & LED_RED) HAL_GPIO_TogglePin(LED_RED_GPIO_Port, LED_RED_Pin);

	if (led & LED_GREEN) HAL_GPIO_TogglePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin);

	if (led & LED_BLUE) HAL_GPIO_TogglePin(LED_BLUE_GPIO_Port, LED_BLUE_Pin);
}

void IO_SetLED(uint8_t led) {
	if (led & LED_RED)
		HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_SET);

	if (led & LED_GREEN)
		HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_SET);

	if (led & LED_BLUE)
		HAL_GPIO_WritePin(LED_BLUE_GPIO_Port, LED_BLUE_Pin, GPIO_PIN_SET);
}

void IO_ResetLED(uint8_t led) {
	if (led & LED_RED)
		HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_RESET);

	if (led & LED_GREEN)
		HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_RESET);

	if (led & LED_BLUE)
		HAL_GPIO_WritePin(LED_BLUE_GPIO_Port, LED_BLUE_Pin, GPIO_PIN_RESET);
}

void IO_Enable_VCC(GPIO_PinState state) {
	HAL_GPIO_WritePin(LOAD_EN_GPIO_Port, LOAD_EN_Pin, state);
}
