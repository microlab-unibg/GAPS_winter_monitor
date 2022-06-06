/**
 ******************************************************************************
 * @file           : LPS22HH.c
 * @brief          : Implementation of LPS22HH driver
 * @author		   : Patrick Locatelli (patrick.locatelli@unibg.it)
 * @version		   : 1.0
 * @date    	   : 13 Feb 2020
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "LPS22HH.h"

/* Private variables ---------------------------------------------------------*/
LPS22HH_Init_t currentConfig;

/*
 * @brief      Read consecutive registers from LPS22HH.
 * @param[in]  Register The first register to read from.
 * @param[out] pReadByte The content of the registers read.
 * @param[in]  Size     The number of consecutive registers to read.
 * @return     The success of the operation.
 */
uint8_t LPS22HH_Read_Reg(uint8_t Register, uint8_t* pReadByte, uint16_t Size) {
	return I2C_Read(I2C_ENV, LPS22HH_ADDRESS, Register, pReadByte, Size)
			== HAL_OK;
}

/*
 * @brief     Write the specified value into the specified register of LPS22HH.
 * @param[in] Register The register to write in.
 * @param[in] Value    The value to write.
 * @return    The success of the operation.
 */
uint8_t LPS22HH_Write_Reg(uint8_t Register, uint8_t Value) {
	uint8_t temp[2];
	temp[0] = Register;
	temp[1] = Value;
	return I2C_Write(I2C_ENV, LPS22HH_ADDRESS, temp, 2) == HAL_OK;

}

/*
 * @brief  Configure the device according to the input structure.
 * @param  pNewConfig Pointer to a LPS22HH_Init_t structure that contains
 *                    the configuration information for the specified device.
 * @return The success of the operation.
 */
uint8_t LPS22HH_Config(LPS22HH_Init_t* pNewConfig) {
	uint8_t result = 1;
	uint8_t temp;

	// CTRL_REG1
	temp = pNewConfig->odr | pNewConfig->bdu;
	result &= LPS22HH_Write_Reg(LPS22HH_REG_CTRL_REG1, temp);

	// CTRL2_XL
	temp = pNewConfig->auto_inc_en | pNewConfig->low_noise_en;
	result &= LPS22HH_Write_Reg(LPS22HH_REG_CTRL_REG2, temp);

	// Check the configuration
	uint8_t check[3];
	LPS22HH_Read_Reg(LPS22HH_REG_CTRL_REG1, check, 2);
	result &= check[0] == (pNewConfig->odr | pNewConfig->bdu)
			&& check[1] == (pNewConfig->auto_inc_en | pNewConfig->low_noise_en);

	if (result) {
		// Store configuration parameters
		memcpy((uint8_t*) &currentConfig, (uint8_t*) pNewConfig,
				sizeof(LPS22HH_Init_t));
	}

	return result;
}

/*
 * @brief      Read the current measured pressure.
 * @param[out] pPressure The pressure expressed in hPa.
 * @return     The success of the operation.
 */
uint8_t LPS22HH_Read_Pressure(float* pPressure) {
	uint8_t result = 1;
	uint8_t buffer[3];
	int32_t temp;

	result &= LPS22HH_Read_Reg(LPS22HH_REG_PRESS_OUT_XL, buffer, 3);
	temp = (((uint16_t) buffer[2]) << 16) | (((uint16_t) buffer[1]) << 8)
			| (uint16_t) buffer[0];
	*pPressure = (float) temp / LPS22HH_PRESS_SENSITIVITY;

	return result;
}

/*
 * @brief      Read the current measured temperature.
 * @param[out] pPressure The pressure expressed in °C.
 * @return     The success of the operation.
 */
uint8_t LPS22HH_Read_Temperature(float* pTemperature) {
	uint8_t result = 1;
	uint8_t buffer[2];
	int16_t temp;

	result &= LPS22HH_Read_Reg(LPS22HH_REG_TEMP_OUT_L, buffer, 2);
	temp = (((uint16_t) buffer[1]) << 8) | (uint16_t) buffer[0];
	*pTemperature = (float) temp / LPS22HH_TEMP_SENSITIVITY;

	return result;
}

/*
 * @brief  Change the value of ODR.
 * @param  NewODR The new ODR value.
 * @return The success of the operation.
 */
uint8_t LPS22HH_Set_ODR(LPS22HH_ODR NewODR) {
	uint8_t result = 1;
	uint8_t temp = 0;

	// Read the current content of CTRL_REG1 register
	result &= LPS22HH_Read_Reg(LPS22HH_REG_CTRL_REG1, &temp, 1);

	// Reset the ODR bits and set the new values
	temp &= 0x0F;
	temp |= NewODR;

	// Write the new value of the register
	result &= LPS22HH_Write_Reg(LPS22HH_REG_CTRL_REG1, temp);

	// Check if the register was correctly written
	result &= LPS22HH_Read_Reg(LPS22HH_REG_CTRL_REG1, &temp, 1);
	result &= (temp & 0x70) == NewODR;

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
uint8_t LPS22HH_PowerDown(uint8_t arg) {
	uint8_t result = 1;
	uint8_t temp;

	// Read the register content
	result &= LPS22HH_Read_Reg(LPS22HH_REG_CTRL_REG1, &temp, 1);

	// Check if accelerometer has already been configured by reading the ODR
	if (arg && (temp & 0x70)) {
		// Device is on: turn it off
		temp &= 0x80;
		result &= LPS22HH_Write_Reg(LPS22HH_REG_CTRL_REG1, temp);
	} else if (!arg && !((temp & 0x70))) {
		// Device is in power down: turn it on
		temp |= currentConfig.odr;
		result &= LPS22HH_Write_Reg(LPS22HH_REG_CTRL_REG1, temp);
	}

	return result;
}

/*
 * @brief  Check the content of the WHO_AM_I register of LSM6DSL.
 * @param  None.
 * @return The success of the operation.
 */
uint8_t LPS22HH_Check_WhoAmI(void) {
	uint8_t temp;
	LPS22HH_Read_Reg(LPS22HH_REG_WHO_AM_I, &temp, 1);
	return temp == LPS22HH_WHO_AM_I_CONTENT;
}
