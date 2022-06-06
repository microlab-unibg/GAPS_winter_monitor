/**
 ******************************************************************************
 * @file    stm32_bluenrg_ble.h
 * @author  CL
 * @version V1.0.0
 * @date    04-July-2014
 * @brief
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; COPYRIGHT(c) 2014 STMicroelectronics</center></h2>
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *   1. Redistributions of source code must retain the above copyright notice,
 *      this list of conditions and the following disclaimer.
 *   2. Redistributions in binary form must reproduce the above copyright notice,
 *      this list of conditions and the following disclaimer in the documentation
 *      and/or other materials provided with the distribution.
 *   3. Neither the name of STMicroelectronics nor the names of its contributors
 *      may be used to endorse or promote products derived from this software
 *      without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 ******************************************************************************
 */

#include "stm32l4xx_hal.h"
#include "main.h"

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32_BLUENRG_BLE_H
#define __STM32_BLUENRG_BLE_H

// SPI Reset Pin
#define BNRG_SPI_RESET_PIN	        BLE_RST_Pin
#define BNRG_SPI_RESET_PORT		    BLE_RST_GPIO_Port

// NSS/CSN/CS: PB.1
#define BNRG_SPI_CS_PIN				BLE_CS_Pin
#define BNRG_SPI_CS_PORT			BLE_CS_GPIO_Port

// IRQ: PA.2
#define BNRG_SPI_IRQ_PIN			BLE_IRQ_Pin
#define BNRG_SPI_IRQ_MODE			GPIO_MODE_IT_RISING
#define BNRG_SPI_IRQ_PULL			GPIO_NOPULL
#define BNRG_SPI_IRQ_SPEED			GPIO_SPEED_HIGH
#define BNRG_SPI_IRQ_ALTERNATE		0
#define BNRG_SPI_IRQ_PORT			BLE_IRQ_GPIO_Port

// EXTI External Interrupt for SPI
// NOTE: if you change the IRQ pin remember to implement a corresponding handler
// function like EXTI0_IRQHandler() in the user project
#define BNRG_SPI_EXTI_IRQn			EXTI2_IRQn
#define BNRG_SPI_EXTI_IRQHandler	EXTI2_IRQHandler
#define BNRG_SPI_EXTI_PIN			BNRG_SPI_IRQ_PIN
#define BNRG_SPI_EXTI_PORT			BNRG_SPI_IRQ_PORT

void Enable_SPI_IRQ(void);
void Disable_SPI_IRQ(void);
void Clear_SPI_IRQ(void);
void Clear_SPI_EXTI_Flag(void);

void BlueNRG_RST(void);
void BlueNRG_HW_Bootloader(void);
int32_t BlueNRG_SPI_Read_All(uint8_t *buffer, uint8_t buff_size);
int32_t BlueNRG_SPI_Write(uint8_t* data1, uint8_t* data2, uint8_t Nb_bytes1,
		uint8_t Nb_bytes2);

#ifdef __cplusplus
}
#endif

#endif /* __STM32_BLUENRG_BLE_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

