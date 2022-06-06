/**
 ******************************************************************************
 * @file           : IO.h
 * @brief          : Header file of IO module
 * @author		   : Patrick Locatelli (patrick.locatelli@unibg.it)
 * @version		   : 1.0
 * @date    	   : 13 Feb 2020
 ******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef IO_LED_H_
#define IO_LED_H_

/* Includes ------------------------------------------------------------------*/
#include "stdint.h"
#include "stm32l4xx_hal_gpio.h"

/* Exported typedef ----------------------------------------------------------*/
typedef enum {
	LED_RED = 0x01,
	LED_GREEN = 0x02,
	LED_BLUE = 0x04
} Led_e;

/* Exported functions prototypes ---------------------------------------------*/
void IO_ToggleLED(uint8_t led);
void IO_SetLED(uint8_t led);
void IO_ResetLED(uint8_t led);
void IO_Enable_VCC(GPIO_PinState state);

#endif /* IO_LED_H_ */
