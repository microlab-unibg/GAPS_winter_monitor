/**
 ******************************************************************************
 * @file           : app.h
 * @brief          : Header file of application
 * @author		   : Patrick Locatelli (patrick.locatelli@unibg.it)
 * @version		   : 1.0
 * @date    	   : 13 Feb 2020
 ******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __APP_H
#define __APP_H

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "../Src/modules/LSM6DSL/LSM6DSL.h"
#include "../Src/modules/HTS221/HTS221.h"
#include "../Src/modules/LPS22HH/LPS22HH.h"
#include "../Src/modules/MAX17048/MAX17048.h"
#include "../Src/modules/io/io.h"
#include "../Src/modules/BLE/ble.h"
#include "../Src/modules/GattDB/comm.h"
#include "../Src/modules/RTC/rtc.h"

/* Exported types ------------------------------------------------------------*/
typedef enum {
	SYSTEM_STATE_STARTUP = 0x00,
	SYSTEM_STATE_INIT = 0x01,
	SYSTEM_STATE_IDLE = 0x10,
	SYSTEM_STATE_IDLE_CONNECTED = 0x14,
	SYSTEM_STATE_SLEEP = 0x17,
	SYSTEM_STATE_LOG = 0x20,
	SYSTEM_STATE_ERROR = 0xFF
} SystemState_e;

typedef enum {
	OP_NONE = 0x00,
	OP_SHUTDOWN = 0x01,
	OP_RESTART = 0x02
} ScheduledOp_e;

typedef struct {
	SystemState_e state;
	SystemState_e oldState;
	uint16_t period;
	uint8_t lprEnabled;
	uint8_t logSensors;
	uint16_t logFrequency;
	uint64_t lastActivityTime;
	ScheduledOp_e scheduledOperation;
	uint8_t cardMounted;
	uint8_t reachedLogBufferEnd;
} Winter_t;

typedef struct {
	uint8_t LSM6DSL;
	uint8_t HTS221;
	uint8_t LPS22HH;
	uint8_t MAX17048;
	uint8_t BLE;
	uint8_t batteryLevel;
} PeripheralsInitStatus_t;

typedef struct {
	uint16_t axlDigits_sample[3];
	float axl_sample[3];
	uint16_t gyroDigits_sample[3];
	float gyro_sample[3];
	float temp_sample;
	float hum_sample;
	float press_sample;
} LatestData_t;

/* Exported functions prototypes ---------------------------------------------*/
uint16_t App_GetPeriod(void);
void App_SetPeriod(uint16_t p);
SystemState_e App_GetSystemState(void);
uint8_t App_SetSystemState(SystemState_e s);
void App_ScheduleOperation(ScheduledOp_e operation);
void App_UpdateLastActivityTime(void);
uint8_t App_StartLog(uint8_t sensors, uint16_t frequency);
uint8_t App_StopLog();
void App_SetLogSensors(uint8_t sensors);
void App_SetLogFrequency(uint8_t frequency);

uint8_t App_InitSystem(void);
void App_DoJob(void);
void App_Handle_SysTick(void);
void App_Handle_DoubleTap(void);
void App_Handle_BTConnected(void);
void App_Handle_BTDisconnected(void);
uint8_t App_DeinitPeripherals(void);
uint8_t App_MyDebug(uint8_t);

/* Exported variables --------------------------------------------------------*/


/* Private defines -----------------------------------------------------------*/
// System states periods
#define PERIOD_INIT			500
#define PERIOD_IDLE			500
#define PERIOD_ERROR		500

// Log sensors
#define SENS_AXL_Pos        (0U)
#define SENS_AXL_Msk        (0x1UL << SENS_AXL_Pos)		/*!< 0x01 */
#define SENS_AXL            SENS_AXL_Msk                /*!<Accelerometer     */
#define SENS_GYRO_Pos       (1U)
#define SENS_GYRO_Msk       (0x1UL << SENS_GYRO_Pos)	/*!< 0x02 */
#define SENS_GYRO           SENS_GYRO_Msk               /*!<Gyroscope     	  */
#define SENS_TEMP_Pos       (2U)
#define SENS_TEMP_Msk       (0x1UL << SENS_TEMP_Pos)	/*!< 0x04 */
#define SENS_TEMP           SENS_TEMP_Msk               /*!<Thermometer       */
#define SENS_HUM_Pos        (3U)
#define SENS_HUM_Msk        (0x1UL << SENS_HUM_Pos)		/*!< 0x08 */
#define SENS_HUM            SENS_HUM_Msk                /*!<Hygrometer     	  */
#define SENS_PRESS_Pos      (4U)
#define SENS_PRESS_Msk      (0x1UL << SENS_PRESS_Pos)	/*!< 0x10 */
#define SENS_PRESS          SENS_PRESS_Msk              /*!<Barometer	      */

// Default values
#define DEFAULT_LOG_PERIOD			200
#define DEFAULT_LOG_SENSORS			(SENS_AXL | SENS_GYRO)

// Main job IRQ
#define JOB_IRQn					TIM6_IRQn
#define JOB_IRQn_Priority			4

// Miscellaneous
/* TODO: adjust this value according to the SYS_CLOCK frequency. */
#define MAXIMUM_LOG_FREQUENCY		100
#define TIMEOUT_IDLE_TO_SLEEP		60000
#define WAKEUP_COUNTER				900		// Represents seconds
#define RESTART_SHUTDOWN_DELAY		2000
#define LED_BLINK_PERIOD_IDLE		10000
#define LED_BLINK_PERIOD_LOG		5000

#endif /* __APP_H */
