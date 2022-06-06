/**
 ******************************************************************************
 * @file           : rtc.h
 * @brief          : Header file of RTC module
 * @author		   : Patrick Locatelli (patrick.locatelli@unibg.it)
 * @version		   : 1.0
 * @date    	   : 06 Mar 2020
 ******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef MODULES_RTC_RTC_H_
#define MODULES_RTC_RTC_H_

/* Includes ------------------------------------------------------------------*/
#include "stdint.h"
#include "main.h"

/* Exported variables --------------------------------------------------------*/
RTC_TimeTypeDef RTC_Time;                   // Current RTC time
RTC_DateTypeDef RTC_Date;                   // Current RTC date

/* Exported functions prototypes ---------------------------------------------*/
uint8_t RTC_SetDateTime(RTC_TimeTypeDef* pTime, RTC_DateTypeDef* pDate);
uint8_t RTC_SetDateTime_Epoch(uint32_t epoch);
uint8_t RTC_GetDateTime(RTC_TimeTypeDef* pTime, RTC_DateTypeDef* pDate);
uint8_t RTC_GetDateTime_Epoch(uint32_t* pEpoch);
uint8_t RTC_GetDateTime_FormattedChar(char* pDateTime);
uint32_t RTC_ToEpoch(RTC_TimeTypeDef* pTime, RTC_DateTypeDef* pDate);
void RTC_FromEpoch(uint32_t epoch, RTC_TimeTypeDef* pTime, RTC_DateTypeDef* pDate);
void RTC_AdjustTimeZone(RTC_TimeTypeDef* pTime, RTC_DateTypeDef* pDate,
		int8_t offset);

/* Private defines -----------------------------------------------------------*/
#define JULIAN_DATE_BASE	2440588	// Unix epoch time in Julian calendar
									// (UnixTime = 00:00:00 01.01.1970 => JDN = 2440588)

#endif /* MODULES_RTC_RTC_H_ */
