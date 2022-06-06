/**
 ******************************************************************************
 * @file           : comm.c
 * @brief          : Implementation of BT communication
 * @author		   : Patrick Locatelli (patrick.locatelli@unibg.it)
 * @version		   : 1.0
 * @date    	   : 06 Mar 2020
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "comm.h"
#include "app.h"
#include "stdio.h"

/* Private defines -----------------------------------------------------------*/
#define CHECK_STATUS_RETURN(x) if (x != BLE_STATUS_SUCCESS) return
#define CHECK_STATUS_RETVAL(x) if (x != BLE_STATUS_SUCCESS) return x

/*
 * Type-Length-Value packet structure
 * 	  Type:   The kind of message (1 Byte).
 * 	  Length: The size of the value field (1 Byte).
 *    Value:  Variable-sized series of bytes which contains data for this part
 * 			  of the message (N Bytes).
 */

uint8_t Evaluate_Command(uint8_t *cmdData, uint8_t dataLen,
		uint8_t* cmdResponse, uint8_t* responseLen) {

	uint8_t read = cmdData[0] & 0x80;
	uint8_t payloadLen = cmdData[1];

	uint8_t updateValue = 1;

	*responseLen = 4;
	cmdResponse[0] = CMD_ACK;		// Response type
	cmdResponse[1] = 2;				// Payload length (default: 2)
	cmdResponse[2] = cmdData[0];	// Requested command
	cmdResponse[3] = ACK_ERROR;		// Response

	switch (cmdData[0] & 0x7F) {
		case CMD_SHUTDOWN:
			break;
		case CMD_STATE:
			if (!read) {
				// Set state
				if (payloadLen == 1) {
					if (App_SetSystemState(cmdData[2])) {
						cmdResponse[3] = ACK_SUCCESS;
					}
				}
			} else {
				// Get state
				if (payloadLen == 0) {
					*responseLen = 5;
					cmdResponse[1] = 3;
					cmdResponse[3] = ACK_SUCCESS;
					cmdResponse[4] = App_GetSystemState();
				}
			}
			break;
		case CMD_RESTART:
			if (!read) {
				if (payloadLen == 0) {
					App_UpdateLastActivityTime();
					App_ScheduleOperation(OP_RESTART);
					cmdResponse[3] = ACK_SUCCESS;
				}
			}
			break;
		case CMD_DATE:
			if (!read) {
				// Set date
				if (payloadLen == 4) {
					uint32_t epoch = ((uint32_t) cmdData[2]) << 24
							| ((uint32_t) cmdData[3]) << 16
							| ((uint32_t) cmdData[4]) << 8
							| ((uint32_t) cmdData[5]);
					if (RTC_SetDateTime_Epoch(epoch)) {
						cmdResponse[3] = ACK_SUCCESS;
					}
				}
			} else {
				// Get date
				if (payloadLen == 0) {
					*responseLen = 8;
					cmdResponse[1] = 6;

					uint32_t epoch;
					if (RTC_GetDateTime_Epoch(&epoch)) {
						cmdResponse[3] = ACK_SUCCESS;
						cmdResponse[4] = (uint8_t) ((epoch & 0xFF000000) >> 24);
						cmdResponse[5] = (uint8_t) ((epoch & 0xFF0000) >> 16);
						cmdResponse[6] = (uint8_t) ((epoch & 0xFF00) >> 8);
						cmdResponse[7] = (uint8_t) (epoch & 0xFF);
					} else {
						cmdResponse[3] = ACK_ERROR;
						cmdResponse[4] = 0;
						cmdResponse[5] = 0;
						cmdResponse[6] = 0;
						cmdResponse[7] = 0;
					}
				}
			}
			break;
		case CMD_START_LOG:
			if (!read) {
				if (payloadLen == 3)
					if (cmdData[2] != 0
							&& (cmdData[3] != 0 || cmdData[4] != 0)) {
						// Check the current state
						if (App_GetSystemState() == SYSTEM_STATE_IDLE_CONNECTED) {
							// Get the log frequency
							uint16_t freq = ((uint16_t) cmdData[3]) << 8
									| (uint16_t) cmdData[4];

							// Start the log of data
							if (App_StartLog(cmdData[2], freq)) {
								cmdResponse[3] = ACK_SUCCESS;
							}
						}
					}
			}
			break;
		case CMD_STOP_LOG:
			if (!read) {
				if (payloadLen == 0) {
					// Check the current state
					if (App_GetSystemState() != SYSTEM_STATE_LOG) {
						cmdResponse[3] = ACK_SUCCESS;
					} else {
						// Stop the log of data
						if (App_StopLog()) {
							cmdResponse[3] = ACK_SUCCESS;
						}
					}
				}
			}
			break;
		default:
			break;
	}

	return updateValue;
}

