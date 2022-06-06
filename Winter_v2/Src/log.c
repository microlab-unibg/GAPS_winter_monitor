/**
 ******************************************************************************
 * @file           : log.c
 * @brief          : Implementation of log functions
 * @author		   : Patrick Locatelli (patrick.locatelli@unibg.it)
 * @version		   : 1.0
 * @date    	   : 06 Mar 2020
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "string.h"
#include "stdio.h"
#include "app.h"
#include "log.h"

/* Private define ------------------------------------------------------------*/
#define FILE_NAME_SIZE			24


/* Private variables ---------------------------------------------------------*/
uint8_t bufferSection;
volatile uint16_t logIndex;
volatile uint8_t little[LITTLE_SIZE];
volatile uint8_t big[BIG_SIZE];
volatile uint16_t bytesToWrite;

char info_path[FILE_NAME_SIZE];
char data_path[FILE_NAME_SIZE];

/* Extern variables ----------------------------------------------------------*/
extern volatile Winter_t winter;
extern volatile LatestData_t latestData;
extern LSM6DSL_Init_t currentAGConfig;
extern DMA_HandleTypeDef hdma_memtomem_dma2_channel1;
extern char SDPath[4];

/* Private function prototypes -----------------------------------------------*/
void appendData(uint8_t* buffer, uint16_t* index);

uint8_t Log_Create_InitStatusFile(PeripheralsInitStatus_t* status) {
	/*// Check the presence of the SD card
	CHECK_SD_CARD_RETVAL;

	// Create a new status file
	char filePath[] = "status.txt";
	retSD = f_open(&SDFile, filePath, FA_WRITE | FA_READ | FA_CREATE_ALWAYS);

	if (retSD == FR_OK) {
		// Write the information
		unsigned int bytesWritten;
		char string[100];

		// Title
		sprintf(string, "*** INITIALIZATION STATUS ***\r\n\r\n");
		retSD |= f_write(&SDFile, string, strlen(string), &bytesWritten);

		// LSM6DSL
		sprintf(string, "LSM6DSL init state:\t");
		retSD |= f_write(&SDFile, string, strlen(string), &bytesWritten);
		sprintf(string, (status->LSM6DSL ? "OK\r\n" : "ERROR\r\n"));
		retSD |= f_write(&SDFile, string, strlen(string), &bytesWritten);

		// HTS221
		sprintf(string, "HTS221 init state:\t");
		retSD |= f_write(&SDFile, string, strlen(string), &bytesWritten);
		sprintf(string, (status->HTS221 ? "OK\r\n" : "ERROR\r\n"));
		retSD |= f_write(&SDFile, string, strlen(string), &bytesWritten);

		// LPS22HH
		sprintf(string, "LPS22HH init state:\t");
		retSD |= f_write(&SDFile, string, strlen(string), &bytesWritten);
		sprintf(string, (status->LPS22HH ? "OK\r\n" : "ERROR\r\n"));
		retSD |= f_write(&SDFile, string, strlen(string), &bytesWritten);

		// BLE
		sprintf(string, "BLE init state:\t");
		retSD |= f_write(&SDFile, string, strlen(string), &bytesWritten);
		sprintf(string, (status->BLE ? "OK\r\n" : "ERROR\r\n"));
		retSD |= f_write(&SDFile, string, strlen(string), &bytesWritten);

		// MAX17048
		sprintf(string, "Battery level:\t");
		retSD |= f_write(&SDFile, string, strlen(string), &bytesWritten);
		utoa(status->MAX17048, string, 10);
		retSD |= f_write(&SDFile, string, strlen(string), &bytesWritten);

		// Close the file
		retSD |= f_close(&SDFile);
	}

	return retSD == FR_OK;
	*/
}

uint8_t Log_Create_DataFile(char* logPath) {
	/*
	// Check the presence of the SD card
	CHECK_SD_CARD_RETVAL;

	retSD = FR_OK;

	// Initialize private variables
	bufferSection = 0;
	logIndex = 0;
	bytesToWrite = 0;

	// Create a new INFO file
	sprintf(info_path, "%s_info.bin", logPath);
	if (f_open(&SDFile, info_path, FA_WRITE | FA_READ | FA_CREATE_ALWAYS)
			== FR_OK) {
		// Write the log session information
		uint8_t info[6];
		info[0] = 33;
		info[1] = winter.logSensors;
		info[2] = currentAGConfig.axl_fs;
		info[3] = currentAGConfig.gyro_fs;
		info[4] = winter.logFrequency;
		info[5] = 33;

		unsigned int bytesWritten;
		retSD |= f_write(&SDFile, info, 6, &bytesWritten);
		retSD |= f_close(&SDFile);
	}

	// Create a new log file
	sprintf(data_path, "%s_data.bin", logPath);
	retSD |= f_open(&SDFile, data_path, FA_WRITE | FA_READ | FA_CREATE_ALWAYS);

	return retSD == FR_OK;
	*/
}

void Log_Write_Data(void) {
	/*
	// Check the presence of the SD card
	CHECK_SD_CARD_RET;

	// Add 4 bytes of fake header
	if (logIndex % 256 == 0) {
		little[logIndex] = '!';
		little[logIndex + 1] = '!';
		little[logIndex + 2] = '!';
		little[logIndex + 3] = '!';
		logIndex += 4;
	}

	// Append the current data
	appendData(little, &logIndex);

	// Check if the "little" buffer is full
	if (logIndex >= LITTLE_SIZE) {
		bytesToWrite += LITTLE_SIZE;
		logIndex = 0;

		// Use DMA to write the content of the "little" buffer into the "big" one
		HAL_DMA_Start_IT(&hdma_memtomem_dma2_channel1,
				(uint32_t) ((uint32_t*) (little)),
				(uint32_t) ((uint32_t*) (big + (bufferSection) * LITTLE_SIZE)),
				LITTLE_SIZE);

		//TODO: deal with LP run mode in advance
//		if(buffer_section + 2 >= BIG_BLOCKS){
//			ExitLowPowerRunMode();
//		}

// Check if the "big" buffer is full
		if (bufferSection + 1 >= BIG_BLOCKS) {
			winter.reachedLogBufferEnd = 1;
			bufferSection = 0;
		} else {
			++bufferSection;
		}
	}
}

uint8_t Log_Close_DataFile(uint8_t forceWriting) {
	// Check the presence of the SD card
	CHECK_SD_CARD_RETVAL;

	if (forceWriting) {
		// TODO: Force the writing of the current content of the buffers
//		bytesToWrite += logIndex;
//		winter.reachedLogBufferEnd = 1;
//
//		HAL_DMA_Start_IT(&hdma_memtomem_dma2_channel1,
//				(uint32_t) ((uint32_t*) (little)),
//				(uint32_t) ((uint32_t*) (big + (bufferSection) * LITTLE_SIZE)),
//				logIndex);
	}

	return f_close(&SDFile) == FR_OK;
	*/
}

void MemToMemTxCplt(DMA_HandleTypeDef *dma) {
	// Check if the "big" buffer has been filled with data
	if (winter.reachedLogBufferEnd) {
		unsigned int bytesWritten;

		// Disable LPR mode
		if (winter.lprEnabled) {
			Disable_LPR_Mode();
			winter.lprEnabled = 0;
		}
		/*
		// Write the content of "big" buffer into the memory
		f_lseek(&SDFile, f_size(&SDFile));
		retSD = f_write(&SDFile, big, bytesToWrite, &bytesWritten);

		if (retSD == FR_OK && bytesWritten > 0) f_sync(&SDFile);
		winter.reachedLogBufferEnd = 0;
		bytesToWrite = 0;
		*/
		// Enable back LPR mode
		if (!winter.lprEnabled) {
			Enable_LPR_Mode();
			winter.lprEnabled = 1;
		}
	}
}

void appendData(uint8_t* buffer, uint16_t* index) {
	if ((winter.logSensors & SENS_AXL) != 0) {
		memcpy(&buffer[*index], latestData.axlDigits_sample, 6);
		*(index) += 6;
	}
	if ((winter.logSensors & SENS_GYRO) != 0) {
		memcpy(&buffer[*index], latestData.gyroDigits_sample, 6);
		*(index) += 6;
	}
}

