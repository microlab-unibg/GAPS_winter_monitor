/**
 ******************************************************************************
 * @file           : HTS221.c
 * @brief          : Implementation of HTS221 driver
 * @author		   : Patrick Locatelli (patrick.locatelli@unibg.it)
 * @version		   : 1.0
 * @date    	   : 13 Feb 2020
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "HTS221.h"
#include "stm32l4xx_hal.h"
#include "main.h"

/* Private variables ---------------------------------------------------------*/
float HTS221_T_slope = 0;
float HTS221_H_slope = 0;
float HTS221_T_offset = 0;
float HTS221_H_offset = 0;

/*
 * @brief      Read consecutive registers from HTS221.
 * @param[in]  Register The first register to read from.
 * @param[out] ReadByte The content of the registers read.
 * @param[in]  Size     The number of consecutive registers to read.
 * @return     The success of the operation.
 */
uint8_t HTS221_Read_Reg(uint8_t Register, uint8_t* ReadByte, uint16_t Size) {
	if (Size > 1)
		Register |= 0x80;
	return I2C_Read(I2C_ENV, HTS221_ADDRESS, Register, ReadByte, Size) == HAL_OK;
}

/*
 * @brief     Write the specified value into the specified register of HTS221.
 * @param[in] Register The register to write in.
 * @param[in] Value    The value to write.
 * @return    The success of the operation.
 */
uint8_t HTS221_Write_Reg(uint8_t Register, uint8_t Value) {
	uint8_t temp[2];
	temp[0] = Register;
	temp[1] = Value;
	return I2C_Write(I2C_ENV, HTS221_ADDRESS, temp, 2) == HAL_OK;
}

/*
 * @brief  Configure the device.
 * @param  None.
 * @return The success of the operation.
 */
uint8_t HTS221_Config(void) {
	uint8_t result = 1;
	uint8_t buffer[4];

	// AV_CONF
	buffer[0] = 0x3F;	// (AVGT: 256, AVGH: 512)
	result &= HTS221_Write_Reg(HTS221_REG_AV_CONF, buffer[0]);

	// CTRL_REG1:
	// 		PD = 0x80 (active mode)
	// 		BDU = 0x04 (non-continuous update)
	// 		ODR = 0x03 (T: 12.5Hz, H: 12.5Hz)
	buffer[0] = 0x80 | 0x04 | 0x03;
	result &= HTS221_Write_Reg(HTS221_REG_CTRL1, buffer[0]);

	/* --- Temperature slope and offset --------------------------------------*/
	uint8_t tmp;
	float t0_degC, t1_degC;
	uint16_t t0_out_U, t1_out_U;
	int16_t t0_out, t1_out;

	// Read T0_degC and T1_degC coefficients (absolute value)
	result &= HTS221_Read_Reg(HTS221_REG_T0_degC_x8, buffer, 2);

	// Read the MSB bits of T0_degC and T1_degC
	result &= HTS221_Read_Reg(HTS221_REG_T1_T0_msb, &tmp, 1);

	// Calculate the T0_degC and T1_degC values
	t0_degC = ((((uint16_t) (tmp & 0x03)) << 8) | ((uint16_t) buffer[0])) >> 3;
	t1_degC = ((((uint16_t) (tmp & 0x0C)) << 6) | ((uint16_t) buffer[1])) >> 3;

	// Read T0_OUT and T1_OUT
	result &= HTS221_Read_Reg(HTS221_REG_T0_OUT_L, buffer, 4);
	t0_out_U = (((uint16_t) buffer[1]) << 8) | (uint16_t) buffer[0];
	t1_out_U = (((uint16_t) buffer[3]) << 8) | (uint16_t) buffer[2];
	t0_out = *((int16_t*) (&t0_out_U));
	t1_out = *((int16_t*) (&t1_out_U));

	if (result) {
		HTS221_T_slope = (t1_degC - t0_degC) / (t1_out - t0_out);
		HTS221_T_offset = t0_degC - t0_out * HTS221_T_slope;
	}

	/* --- Humidity slope and offset -----------------------------------------*/
	float h0_rh, h1_rh;
	uint16_t h0_t0_out_U, h1_t0_out_U;
	int16_t h0_t0_out, h1_t0_out;

	// Read H0_rH and H1_rH coefficients
	result &= HTS221_Read_Reg(HTS221_REG_H0_rH_x2, buffer, 2);
	h0_rh = buffer[0] >> 1;		// divide by 2
	h1_rh = buffer[1] >> 1;		// divide by 2

	// Read H0_T0_OUT
	result &= HTS221_Read_Reg(HTS221_REG_H0_T0_OUT_L, buffer, 2);
	h0_t0_out_U = (((uint16_t) buffer[1]) << 8) | (uint16_t) buffer[0];
	h0_t0_out = *((int16_t*) (&h0_t0_out_U));

	// Read H1_T0_OUT
	result &= HTS221_Read_Reg(HTS221_REG_H1_T0_OUT_L, buffer, 2);
	h1_t0_out_U = (((uint16_t) buffer[1]) << 8) | (uint16_t) buffer[0];
	h1_t0_out = *((int16_t*) (&h1_t0_out_U));

	// Define the slope and the offset
	if (result) {
		HTS221_H_slope = (h1_rh - h0_rh) / (h1_t0_out - h0_t0_out);
		HTS221_H_offset = h0_rh - h0_t0_out * HTS221_H_slope;
	}

	return result;
}

/*
 * @brief  Read the current measured temperature value.
 * @param  temperature The temperature value expressed in °C.
 * @return The success of the operation.
 */
uint8_t HTS221_Read_Temperature(float* temperature) {
	uint8_t result = 1;
	uint8_t buffer[2];
	int16_t temp;

	result &= HTS221_Read_Reg(HTS221_REG_TEMP_OUT_L, buffer, 2);
	temp = (((uint16_t) buffer[1]) << 8) | (uint16_t) buffer[0];
	*temperature = temp * HTS221_T_slope + HTS221_T_offset;

	return result;
}

/*
 * @brief  Read the current measured humidity value.
 * @param  humidity The humidity value expressed in rH %.
 * @return The success of the operation.
 */
uint8_t HTS221_Read_Humidity(float* humidity) {
	uint8_t result = 1;
	uint8_t buffer[2];
	int16_t temp;

	result &= HTS221_Read_Reg(HTS221_REG_HUM_OUT_L, buffer, 2);
	temp = (((uint16_t) buffer[1]) << 8) | (uint16_t) buffer[0];
	*humidity = temp * HTS221_H_slope + HTS221_H_offset;

	// Check saturation condition
	if (*humidity > 100) *humidity = 100;

	return result;
}

/*
 * @brief  Change the power-down state of the device.
 * @param  arg A value indicating whether or not to power down the device.
 * 			   This parameter can be one of the following values:
 *             @arg 0: turn on the device (in case it was powered off)
 *             @arg 1: turn off the device (in case it was powered on)
 * @return The success of the operation.
 */
uint8_t HTS221_PowerDown(uint8_t arg) {
	uint8_t result = 1;
	uint8_t temp;

	// Read the register content
	result &= HTS221_Read_Reg(HTS221_REG_CTRL1, &temp, 1);

	// Check if accelerometer has already been configured by reading the ODR
	if (arg && (temp & 0x80)) {
		// Device is on: turn it off
		temp &= 0x7F;
		result &= HTS221_Write_Reg(HTS221_REG_CTRL1, temp);
	} else if (!arg && !((temp & 0x80))) {
		// Device is in power down: turn it on
		temp |= 0x80;
		result &= HTS221_Write_Reg(HTS221_REG_CTRL1, temp);
	}

	return result;
}

/*
 * @brief  Check the content of the WHO_AM_I register of HTS221.
 * @param  None.
 * @return The success of the operation.
 */
uint8_t HTS221_Check_WhoAmI(void) {
	uint8_t temp;
	HTS221_Read_Reg(HTS221_REG_WHO_AM_I, &temp, 1);
	return temp == HTS221_WHO_AM_I_CONTENT;
}
