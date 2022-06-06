/**
 ******************************************************************************
 * @file           : log.h
 * @brief          : Header file of log functions
 * @author		   : Patrick Locatelli (patrick.locatelli@unibg.it)
 * @version		   : 1.0
 * @date    	   : 06 Mar 2020
 ******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __LOG_H
#define __LOG_H

/* Includes ------------------------------------------------------------------*/
#include "stdint.h"
#include "stm32l4xx_hal_dma.h"

/* Private defines -----------------------------------------------------------*/
#define SD_BLOCK_SIZE 		512
#define LITTLE_BLOCKS		1	//FIXME: for now only 1 is allowed
#define LITTLE_SIZE			SD_BLOCK_SIZE * LITTLE_BLOCKS
#define BIG_BLOCKS			10
#define BIG_SIZE			SD_BLOCK_SIZE * BIG_BLOCKS
//TODO: check the previous values

/* Exported functions prototypes ---------------------------------------------*/
uint8_t Log_Create_InitStatusFile(PeripheralsInitStatus_t* status);
uint8_t Log_Create_DataFile(char* logPath);
void Log_Write_Data(void);
//uint8_t Log_Close_DataFile(uint8_t forceWriting);
void MemToMemTxCplt(DMA_HandleTypeDef *dma);

#endif /* __LOG_H */
