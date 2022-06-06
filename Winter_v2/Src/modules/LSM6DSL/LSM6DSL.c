/**
 ******************************************************************************
 * @file           : LSM6DSL.c
 * @brief          : Implementation of LSM6DSL driver
 * @author		   : Patrick Locatelli (patrick.locatelli@unibg.it)
 * @version		   : 1.0
 * @date    	   : 13 Feb 2020
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "string.h"
#include "LSM6DSL.h"

/* Private variables ---------------------------------------------------------*/
LSM6DSL_Init_t currentAGConfig;

/* Private function prototypes -----------------------------------------------*/
LSM6DSL_ODR Get_ODR(uint8_t Register);
uint8_t Set_ODR(uint8_t Register, LSM6DSL_ODR NewODR);
void Adjust_Axl_Sensitivity(LSM6DSL_FS_XL fs);
void Adjust_Gyro_Sensitivity(LSM6DSL_FS_G fs);

/*
 * @brief      Read consecutive registers from LSM6DSL.
 * @param[in]  Register  The first register to read from.
 * @param[out] pReadByte The content of the registers read.
 * @param[in]  Size      The number of consecutive registers to read.
 * @return     The success of the operation.
 */
uint8_t LSM6DSL_Read_Reg(uint8_t Register, uint8_t* pReadByte, uint16_t Size) {
	uint8_t reg = Register | 0x80;	// OR with READ bit (1)
	return SPI_Read(SPI_MOTION, reg, pReadByte, Size) == HAL_OK;
}

/*
 * @brief     Write the specified value into the specified register of LSM6DSL.
 * @param[in] Register The register to write in.
 * @param[in] Value    The value to write.
 * @return    The success of the operation.
 */
uint8_t LSM6DSL_Write_Reg(uint8_t Register, uint8_t Value) {
	uint8_t temp[2];
	temp[0] = Register | 0x00;	// OR with WRITE bit (0)
	temp[1] = Value;
	return SPI_Write(SPI_MOTION, temp, 2) == HAL_OK;
}

/*
 * @brief  Configure the device according to the input structure.
 * @param  pNewConfig Pointer to a LSM6DSL_Init_t structure that contains
 *                    the configuration information for the specified device.
 * @return The success of the operation.
 */
uint8_t LSM6DSL_Config(LSM6DSL_Init_t* pNewConfig) {
	uint8_t result = 1;
	uint8_t temp;

	// CTRL1_XL
	temp = pNewConfig->axl_odr | pNewConfig->axl_fs;
	result &= LSM6DSL_Write_Reg(LSM6DSL_REG_CTRL1_XL, temp);

	// CTRL2_G
	temp = pNewConfig->gyro_odr | pNewConfig->gyro_fs;
	result &= LSM6DSL_Write_Reg(LSM6DSL_REG_CTRL2_G, temp);

	// CTRL3_C
	temp = pNewConfig->bdu;
	result &= LSM6DSL_Write_Reg(LSM6DSL_REG_CTRL3_C, temp);

	// CTRL6_C
	temp = pNewConfig->axl_mode;
	result &= LSM6DSL_Write_Reg(LSM6DSL_REG_CTRL6_C, temp);

	// CTRL7_C
	temp = pNewConfig->gyro_mode;
	result &= LSM6DSL_Write_Reg(LSM6DSL_REG_CTRL7_C, temp);

	// TAP_CFG
	temp = pNewConfig->int_enable | pNewConfig->int_enabledAxis
			| pNewConfig->int_lir;
	result &= LSM6DSL_Write_Reg(LSM6DSL_REG_TAP_CFG, temp);

	// TAP_THS_6D
	temp = (pNewConfig->int_tapThs & 0x1F) | 0x80;	// disable 4D detection
	result &= LSM6DSL_Write_Reg(LSM6DSL_REG_TAP_THS_6D, temp);

	// INT_DUR2
	temp = ((pNewConfig->int_tapDur & 0x0F) << LSM6DSL_REG_INT_DUR2_DUR_SHIFT)
			| ((pNewConfig->int_tapQuiet & 0x03)
					<< LSM6DSL_REG_INT_DUR2_QUIET_SHIFT)
			| ((pNewConfig->int_tapShock & 0x03)
					<< LSM6DSL_REG_INT_DUR2_SHOCK_SHIFT);
	result &= LSM6DSL_Write_Reg(LSM6DSL_REG_INT_DUR2, temp);

	// WAKE_UP_THS
	temp = pNewConfig->int_SDTapEnable;
	result &= LSM6DSL_Write_Reg(LSM6DSL_REG_WAKE_UP_THS, temp);

	// MD1_CFG
	temp = pNewConfig->int1_dt;
	result &= LSM6DSL_Write_Reg(LSM6DSL_REG_MD1_CFG, temp);

	// Check the configuration
	uint8_t check[4];
	LSM6DSL_Read_Reg(LSM6DSL_REG_CTRL1_XL, check, 3);
	result &= check[0] == (pNewConfig->axl_odr | pNewConfig->axl_fs)
			&& check[1] == (pNewConfig->gyro_odr | pNewConfig->gyro_fs)
			&& check[2] == pNewConfig->bdu;
	LSM6DSL_Read_Reg(LSM6DSL_REG_CTRL6_C, check, 2);
	result &= check[0] == pNewConfig->axl_mode
			&& check[1] == pNewConfig->gyro_mode;
	LSM6DSL_Read_Reg(LSM6DSL_REG_TAP_CFG, check, 4);
	result &= check[0]
			== (pNewConfig->int_enable | pNewConfig->int_enabledAxis
					| pNewConfig->int_lir)
			&& check[1] == ((pNewConfig->int_tapThs & 0x1F) | 0x80)
			&& check[2]
					== (((pNewConfig->int_tapDur & 0x0F)
							<< LSM6DSL_REG_INT_DUR2_DUR_SHIFT)
							| ((pNewConfig->int_tapQuiet & 0x03)
									<< LSM6DSL_REG_INT_DUR2_QUIET_SHIFT)
							| ((pNewConfig->int_tapShock & 0x03)
									<< LSM6DSL_REG_INT_DUR2_SHOCK_SHIFT))
			&& check[3] == pNewConfig->int_SDTapEnable;
	LSM6DSL_Read_Reg(LSM6DSL_REG_MD1_CFG, check, 1);
	result &= check[0] == pNewConfig->int1_dt;

	if (result) {
		// Store configuration parameters
		memcpy((uint8_t*) &currentAGConfig, (uint8_t*) pNewConfig,
				sizeof(LSM6DSL_Init_t));

		// Define the sensors' sensitivities on the basis of the full scales
		Adjust_Axl_Sensitivity(currentAGConfig.axl_fs);
		Adjust_Gyro_Sensitivity(currentAGConfig.gyro_fs);
	}

	return result;
}

/*
 * @brief      Read the current measured acceleration.
 * @param[out] pAxl_digits The acceleration expressed in ADC digits.
 * @param[out] pAxl        The acceleration expressed in mg.
 * @return     The success of the operation.
 */
uint8_t LSM6DSL_Read_Axl(uint16_t* pAxl_digits, float* pAxl) {
	uint8_t result = 1;
	uint8_t buffer[6];
	uint16_t temp_data;

	// Read the registers content
	result &= LSM6DSL_Read_Reg(LSM6DSL_REG_OUTX_L_XL, buffer, 6);

	// Cast and return the acceleration values
	for (uint8_t i = 0; i < 3; i++) {
		temp_data = (((uint16_t) buffer[i * 2 + 1]) << 8)
				| (uint16_t) buffer[i * 2];
		pAxl_digits[i] = temp_data;
		pAxl[i] = (int16_t) (*((int16_t*) &temp_data));
		pAxl[i] *= currentAGConfig.axl_sensitivity;
	}

	return result;
}

/*
 * @brief      Read the current measured angular rate.
 * @param[out] pGyro_digits The angular rate expressed in ADC digits.
 * @param[out] pGyro        The angular rate expressed in mdps.
 * @return     The success of the operation.
 */
uint8_t LSM6DSL_Read_Gyro(uint16_t* pGyro_digits, float* pGyro) {
	uint8_t result = 1;
	uint8_t buffer[6];
	uint16_t temp_data;

	// Read the registers content
	result &= LSM6DSL_Read_Reg(LSM6DSL_REG_OUTX_L_G, buffer, 6);

	// Cast and return the angular rate values
	for (uint8_t i = 0; i < 3; i++) {
		temp_data = (((uint16_t) buffer[i * 2 + 1]) << 8)
				| (uint16_t) buffer[i * 2];
		pGyro_digits[i] = temp_data;
		pGyro[i] = (int16_t) (*((int16_t*) &temp_data));
		pGyro[i] *= currentAGConfig.gyro_sensitivity;
	}

	return result;
}

/*
 * @brief  Change the power-down state of the accelerometer.
 * @param  arg A value indicating whether or not to power down the device.
 * 			   This parameter can be one of the following values:
 *             @arg 0: turn on the device (in case it was powered off)
 *             @arg 1: turn off the device (in case it was powered on)
 * @return The success of the operation.
 */
uint8_t LSM6DSL_PowerDown_Axl(uint8_t arg) {
	uint8_t result = 1;
	uint8_t temp;

	// Read the register content
	result &= LSM6DSL_Read_Reg(LSM6DSL_REG_CTRL1_XL, &temp, 1);

	// Check if accelerometer has already been configured by reading the ODR
	if (arg && (temp & 0xF0)) {
		// Accelerometer is configured
		temp &= 0x0F;
		result &= LSM6DSL_Write_Reg(LSM6DSL_REG_CTRL1_XL, temp);
	} else if (!arg && !((temp & 0xF0))) {
		// Accelerometer is in power down
		temp |= currentAGConfig.axl_odr;
		result &= LSM6DSL_Write_Reg(LSM6DSL_REG_CTRL1_XL, temp);
	}

	return result;
}

/*
 * @brief  Change the power-down state of the gyroscope.
 * @param  arg A value indicating whether or not to power down the device.
 * 			   This parameter can be one of the following values:
 *             @arg 0: turn on the device (in case it was powered off)
 *             @arg 1: turn off the device (in case it was powered on)
 * @return The success of the operation.
 */
uint8_t LSM6DSL_PowerDown_Gyro(uint8_t arg) {
	uint8_t result = 1;
	uint8_t temp;

	// Read the register content
	result &= LSM6DSL_Read_Reg(LSM6DSL_REG_CTRL2_G, &temp, 1);

	// Check if gyroscope has already been configured by reading the ODR
	if (arg && (temp & 0xF0)) {
		// Gyro is configured
		temp &= 0x0F;
		result &= LSM6DSL_Write_Reg(LSM6DSL_REG_CTRL2_G, temp);
	} else if (!arg && !((temp & 0xF0))) {
		// Gyro is in power down
		temp |= currentAGConfig.gyro_odr;
		result &= LSM6DSL_Write_Reg(LSM6DSL_REG_CTRL2_G, temp);
	}

	return result;
}

/*
 * @brief  Get the ODR of the accelerometer.
 * @param  None.
 * @return The success of the operation.
 */
LSM6DSL_ODR LSM6DSL_Get_Axl_ODR(void) {
	return Get_ODR(LSM6DSL_REG_CTRL1_XL);
}

/*
 * @brief  Change the ODR of the accelerometer.
 * @param  NewODR The new ODR value.
 * @return The success of the operation.
 */
uint8_t LSM6DSL_Set_Axl_ODR(LSM6DSL_ODR NewODR) {
	uint8_t result = Set_ODR(LSM6DSL_REG_CTRL1_XL, NewODR);
	if (result) currentAGConfig.axl_odr = NewODR;
	return result;
}

/*
 * @brief  Get the ODR of the gyroscope.
 * @param  None.
 * @return The success of the operation.
 */
LSM6DSL_ODR LSM6DSL_Get_Gyro_ODR(void) {
	return Get_ODR(LSM6DSL_REG_CTRL2_G);
}

/*
 * @brief  Change the ODR of the gyroscope.
 * @param  NewODR The new ODR value.
 * @return The success of the operation.
 */
uint8_t LSM6DSL_Set_Gyro_ODR(LSM6DSL_ODR NewODR) {
	uint8_t result = Set_ODR(LSM6DSL_REG_CTRL2_G, NewODR);
	if (result) currentAGConfig.gyro_odr = NewODR;
	return result;
}

uint8_t LSM6DSL_Enable_DoubleTapInterrupt(uint8_t arg) {
	return LSM6DSL_Write_Reg(LSM6DSL_REG_WAKE_UP_THS,
			(arg ? LSM6DSL_SDTAP_ENABLED : LSM6DSL_SDTAP_DISABLED));
}

uint8_t LSM6DSL_Reset_DoubleTapInterrupt(void) {
	uint8_t readByte;
	return LSM6DSL_Read_Reg(LSM6DSL_REG_TAP_SRC, &readByte, 1);
}

/*
 * @brief  Check the content of the WHO_AM_I register of LSM6DSL.
 * @param  None.
 * @return The success of the operation.
 */
uint8_t LSM6DSL_Check_WhoAmI(void) {
	uint8_t temp;
	LSM6DSL_Read_Reg(LSM6DSL_REG_WHO_AM_I, &temp, 1);
	return temp == LSM6DSL_WHO_AM_I_CONTENT;
}

/*
 * @brief  Get the ODR value in the specified register.
 * @param  Register The register to be read.
 * @return The success of the operation.
 */
LSM6DSL_ODR Get_ODR(uint8_t Register) {
	uint8_t temp = 0;

	// Read the current content of the related register
	LSM6DSL_Read_Reg(Register, &temp, 1);
	return (LSM6DSL_ODR) (temp & 0xF0);
}

/*
 * @brief  Change the value of ODR in the specified register.
 * @param  Register The register to be modified.
 * @param  NewODR   The new ODR value.
 * @return The success of the operation.
 */
uint8_t Set_ODR(uint8_t Register, LSM6DSL_ODR NewODR) {
	uint8_t result = 1;
	uint8_t temp = 0;

	// Read the current content of the related register
	result &= LSM6DSL_Read_Reg(Register, &temp, 1);

	// Reset the ODR bits and set the new values
	temp &= 0x0F;
	temp |= NewODR;

	// Write the new value of the register
	result &= LSM6DSL_Write_Reg(Register, temp);

	// Check if the register was correctly written
	result &= LSM6DSL_Read_Reg(Register, &temp, 1);
	result &= (temp & 0xF0) == NewODR;

	return result;
}

/*
 * @brief  Change the accelerometer sensitivity according to the full scale.
 * @param  fs The full scale of the accelerometer.
 */
void Adjust_Axl_Sensitivity(LSM6DSL_FS_XL fs) {
	switch (fs) {
		case LSM6DSL_FS_XL_2g:
			currentAGConfig.axl_sensitivity = LSM6DSL_AXL_SENSITIVITY_2g;
			break;
		case LSM6DSL_FS_XL_16g:
			currentAGConfig.axl_sensitivity = LSM6DSL_AXL_SENSITIVITY_16g;
			break;
		case LSM6DSL_FS_XL_4g:
			currentAGConfig.axl_sensitivity = LSM6DSL_AXL_SENSITIVITY_4g;
			break;
		case LSM6DSL_FS_XL_8g:
			currentAGConfig.axl_sensitivity = LSM6DSL_AXL_SENSITIVITY_8g;
			break;
		default:
			currentAGConfig.axl_sensitivity = LSM6DSL_AXL_SENSITIVITY_2g;
			break;
	}
}

/*
 * @brief  Change the gyroscope sensitivity according to the full scale.
 * @param  fs The full scale of the gyroscope.
 */
void Adjust_Gyro_Sensitivity(LSM6DSL_FS_G fs) {
	switch (fs) {
		case LSM6DSL_FS_G_125dps:
			currentAGConfig.gyro_sensitivity = LSM6DSL_GYRO_SENSITIVITY_125dps;
			break;
		case LSM6DSL_FS_G_250dps:
			currentAGConfig.gyro_sensitivity = LSM6DSL_GYRO_SENSITIVITY_250dps;
			break;
		case LSM6DSL_FS_G_500dps:
			currentAGConfig.gyro_sensitivity = LSM6DSL_GYRO_SENSITIVITY_500dps;
			break;
		case LSM6DSL_FS_G_1000dps:
			currentAGConfig.gyro_sensitivity = LSM6DSL_GYRO_SENSITIVITY_1000dps;
			break;
		case LSM6DSL_FS_G_2000dps:
			currentAGConfig.gyro_sensitivity = LSM6DSL_GYRO_SENSITIVITY_2000dps;
			break;
		default:
			currentAGConfig.gyro_sensitivity = LSM6DSL_GYRO_SENSITIVITY_250dps;
			break;
	}

	currentAGConfig.gyro_sensitivity /= 1000;
}
