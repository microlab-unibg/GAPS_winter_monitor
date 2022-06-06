/**
 ******************************************************************************
 * @file           : comm.h
 * @brief          : Header file of BT communication
 * @author		   : Patrick Locatelli (patrick.locatelli@unibg.it)
 * @version		   : 1.0
 * @date    	   : 06 Mar 2020
 ******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef MODULES_GATTDB_COMM_H_
#define MODULES_GATTDB_COMM_H_

/* Includes ------------------------------------------------------------------*/
#include "stdint.h"

/* Exported types ------------------------------------------------------------*/
typedef enum {
	CMD_SHUTDOWN = 0x01,		//!< CMD_SHUTDOWN
	CMD_STATE = 0x02,			//!< CMD_STATE (SET/GET THE STATE)
	CMD_RESTART = 0x03,     	//!< CMD_RESTART
	CMD_DATE = 0x04,			//!< CMD_DATE (SET/GET THE STATE)
	CMD_START_LOG = 0x05,		//!< CMD_START_LOG
	CMD_STOP_LOG = 0x06,		//!< CMD_STOP_LOG
	CMD_ACK = 0x00				//!< CMD_ACK
} Command_e; //Command

typedef enum {
	ACK_SUCCESS = 0x00,
	ACK_ERROR = 0x01
} Ack_t;

/* Exported functions prototypes ---------------------------------------------*/
uint8_t Evaluate_Command(uint8_t *cmdData, uint8_t dataLen,
		uint8_t* cmdResponse, uint8_t* responseLen);

#endif /* MODULES_GATTDB_COMM_H_ */
