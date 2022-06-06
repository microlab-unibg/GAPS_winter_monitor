#ifndef MODULES_BLE_BLE_H_
#define MODULES_BLE_BLE_H_

#include "stdint.h"
#include "stm32l4xx_hal.h"
#include "stm32l4xx_it.h"

/**
 * @file    BLE.h
 * @author  Daniele Comotti - daniele.comotti@unibg.it daniele.comotti@221e.it
 * @version V1.0 alpha
 * @date    07 November 2016
 * @brief   BLE header file
 * @details Edit this file at your own risk
 * Università  degli studi di Bergamo, Microelectronics Laboratory - 221e srl
 */

// Connection period, arg in msec.
#define CONN_P(x)   ((int)((x)/1.25f))
#define CONN_P1     (CONN_P(40))
#define CONN_P2     (CONN_P(40))
#define SCAN_P 		(0x200)
#define SCAN_L 		(0x200)

#define BLE_JOB_IRQn			TIM2_IRQn
#define BLE_JOB_IRQn_Priority 4
//#define BLE_JOB_IRQHandler	TIM2_IRQHandler

#define BLE_NAME_LEN	15 // Max length of the BLE name in bytes, '\0' included

#define BNRG_SPI_EXTI_IRQn_Priority 2

typedef struct {
	uint8_t connected;
	uint8_t bd_address[6];
	uint8_t state;
	char name[BLE_NAME_LEN];
} BLE_TypeDef;

////////////////////////////
#define BLE_CMD_MAX_LEN		16

typedef struct {
	uint8_t updateChar;
	uint8_t respValue[BLE_CMD_MAX_LEN];
	uint8_t respLen;
} BLE_Config;

uint8_t BLE_Init(void);
void BLE_DeInit(void);
uint8_t BLE_Services_Init(void);
uint8_t BLE_GetState(void);
void BLE_GetBDAddress(uint8_t* bd);
uint8_t BLE_SetName(uint32_t* arg);
void BLE_SetConnectable(void);
void BLE_SetIRQs(uint8_t arg);
void BLE_Refresh(void);
void BLE_DoJob(void);

uint8_t BLE_IsConnected(void);

void GAP_ConnectionComplete_CB(uint8_t addr[6], uint16_t handle);
void GAP_DisconnectionComplete_CB(void);

#endif /* MODULES_BLE_BLE_H_ */
