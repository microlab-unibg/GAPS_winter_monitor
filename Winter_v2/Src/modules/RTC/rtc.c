/**
 ******************************************************************************
 * @file           : rtc.h
 * @brief          : Header file of RTC module
 * @author		   : Patrick Locatelli (patrick.locatelli@unibg.it)
 * @version		   : 1.0
 * @date    	   : 06 Mar 2020
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "stdio.h"
#include "rtc.h"

/* Extern variables ----------------------------------------------------------*/
extern RTC_HandleTypeDef hrtc;

/*
 * @brief  Set the date and time.
 * @param[in] pTime Pointer to a RTC_TimeTypeDef structure which contains the
 * 					current time.
 * @param[in] pDate Pointer to a RTC_DateTypeDef structure which contains the
 * 					current date.
 * @return The success of the operation.
 */
uint8_t RTC_SetDateTime(RTC_TimeTypeDef* pTime, RTC_DateTypeDef* pDate) {
	uint8_t result = 0;
	result |= HAL_RTC_SetTime(&hrtc, pTime, RTC_FORMAT_BIN);
	result |= HAL_RTC_SetDate(&hrtc, pDate, RTC_FORMAT_BIN);
	return result == HAL_OK;
}

/*
 * @brief  Set the date and time from a UNIX time.
 * @param  epoch A date and time expressed in UNIX time (number of seconds from
 * 				 01 Jan 1970).
 * @return The success of the operation.
 */
uint8_t RTC_SetDateTime_Epoch(uint32_t epoch) {
	RTC_FromEpoch(epoch, &RTC_Time, &RTC_Date);
	return RTC_SetDateTime(&RTC_Time, &RTC_Date);
}

/*
 * @brief  	   Get the current date and time.
 * @param[out] pTime Pointer to a RTC_TimeTypeDef structure which will contain
 * 					 the current time.
 * @param[out] pDate Pointer to a RTC_DateTypeDef structure which will contain
 * 					 the current date.
 * @return 	   The success of the operation.
 */
uint8_t RTC_GetDateTime(RTC_TimeTypeDef* pTime, RTC_DateTypeDef* pDate) {
	uint8_t result = 0;
	result |= HAL_RTC_GetTime(&hrtc, pTime, RTC_FORMAT_BIN);
	result |= HAL_RTC_GetDate(&hrtc, pDate, RTC_FORMAT_BIN);
	return result == HAL_OK;
}

/*
 * @brief  	   Get the current date and time in UNIX time.
 * @param[out] pEpoch The current date and time expressed in UNIX time
 * 					  (number of seconds from 01 Jan 1970).
 * @return 	   The success of the operation.
 */
uint8_t RTC_GetDateTime_Epoch(uint32_t* pEpoch) {
	uint8_t result = RTC_GetDateTime(&RTC_Time, &RTC_Date);
	*pEpoch = RTC_ToEpoch(&RTC_Time, &RTC_Date);
	return result;
}

/*
 * @brief  	   Get the current date and time as a char array.
 * @param[out] pDateTime The current date and time as a char array formatted as
 * 					  	 yyyymmdd_HHMMSS
 * @return 	   The success of the operation.
 */
uint8_t RTC_GetDateTime_FormattedChar(char* pDateTime) {
	uint8_t result = RTC_GetDateTime(&RTC_Time, &RTC_Date);
	sprintf(pDateTime, "%04d%02d%02d_%02d%02d%02d", (2000 + RTC_Date.Year),
			RTC_Date.Month, RTC_Date.Date, RTC_Time.Hours, RTC_Time.Minutes,
			RTC_Time.Seconds);
	return result;
}

// Convert Date/Time structures to epoch time
uint32_t RTC_ToEpoch(RTC_TimeTypeDef* pTime, RTC_DateTypeDef* pDate) {
	uint8_t a;
	uint16_t y;
	uint8_t m;
	uint32_t JDN;

	// These hardcore math's are taken from http://en.wikipedia.org/wiki/Julian_day

	// Calculate some coefficients
	a = (14 - pDate->Month) / 12;
	y = (pDate->Year + 2000) + 4800 - a; // years since 1 March, 4801 BC
	m = pDate->Month + (12 * a) - 3; // since 1 March, 4801 BC

	// Gregorian calendar date compute
	JDN = pDate->Date;
	JDN += (153 * m + 2) / 5;
	JDN += 365 * y;
	JDN += y / 4;
	JDN += -y / 100;
	JDN += y / 400;
	JDN = JDN - 32045;
	JDN = JDN - JULIAN_DATE_BASE;    // Calculate from base date
	JDN *= 86400;                     // Days to seconds
	JDN += pTime->Hours * 3600;    // ... and today seconds
	JDN += pTime->Minutes * 60;
	JDN += pTime->Seconds;

	return JDN;
}

// Convert epoch time to Date/Time structures
void RTC_FromEpoch(uint32_t epoch, RTC_TimeTypeDef* pTime, RTC_DateTypeDef* pDate) {
	uint32_t tm;
	uint32_t t1;
	uint32_t a;
	uint32_t b;
	uint32_t c;
	uint32_t d;
	uint32_t e;
	uint32_t m;
	int16_t year = 0;
	int16_t month = 0;
	int16_t dow = 0;
	int16_t mday = 0;
	int16_t hour = 0;
	int16_t min = 0;
	int16_t sec = 0;
	uint64_t JD = 0;
	uint64_t JDN = 0;

	// These hardcore math's are taken from http://en.wikipedia.org/wiki/Julian_day

	JD = ((epoch + 43200) / (86400 >> 1)) + (2440587 << 1) + 1;
	JDN = JD >> 1;

	tm = epoch;
	t1 = tm / 60;
	sec = tm - (t1 * 60);
	tm = t1;
	t1 = tm / 60;
	min = tm - (t1 * 60);
	tm = t1;
	t1 = tm / 24;
	hour = tm - (t1 * 24);

	dow = JDN % 7;
	a = JDN + 32044;
	b = ((4 * a) + 3) / 146097;
	c = a - ((146097 * b) / 4);
	d = ((4 * c) + 3) / 1461;
	e = c - ((1461 * d) / 4);
	m = ((5 * e) + 2) / 153;
	mday = e - (((153 * m) + 2) / 5) + 1;
	month = m + 3 - (12 * (m / 10));
	year = (100 * b) + d - 4800 + (m / 10);

	pDate->Year = year - 2000;
	pDate->Month = month;
	pDate->Date = mday;
	pDate->WeekDay = dow;
	pTime->Hours = hour;
	pTime->Minutes = min;
	pTime->Seconds = sec;
}

// Adjust time with time zone offset
// input:
//   time - pointer to RTC_Time structure with time to adjust
//   date - pointer to RTC_Date structure with date to adjust
//   offset - hours offset to add or subtract from date/time (hours)
void RTC_AdjustTimeZone(RTC_TimeTypeDef* pTime, RTC_DateTypeDef* pDate,
		int8_t offset) {
	uint32_t epoch = RTC_ToEpoch(pTime, pDate) + offset * 3600;
	RTC_FromEpoch(epoch, pTime, pDate);
}
