/**
 ******************************************************************************
 * @file           : MAX17048.c
 * @brief          : Implementation of MAX17048 driver
 * @author		   : Patrick Locatelli (patrick.locatelli@unibg.it)
 * @version		   : 1.0
 * @date    	   : 14 Feb 2020
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "stm32l4xx_hal.h"
#include "main.h"
#include "MAX17048.h"

/* Private variables ---------------------------------------------------------*/
uint16_t MAX17048_ADDR = 0x6C;

/*
 * @brief      Read consecutive registers from MAX17048.
 * @param[in]  Register The first register to read from.
 * @param[out] ReadByte The content of the registers read.
 * @param[in]  Size     The number of consecutive registers to read.
 * @return     The success of the operation.
 */
uint8_t MAX17048_ReadReg(uint8_t Register, uint8_t* ReadByte, uint16_t Size) {
	return I2C_Read(I2C_GAS, MAX17048_ADDR, Register, ReadByte, Size) == HAL_OK;
}

/*
 * @brief     Write the specified value into the specified register of MAX17048.
 * @param[in] Register The register to write in.
 * @param[in] Value    The value to write.
 * @return    The success of the operation.
 */
uint8_t MAX17048_WriteReg(uint8_t Register, uint16_t Value) {
	uint8_t temp[3];
	uint8_t* pValue = ((uint8_t*) (&Value));
	temp[0] = Register;
	temp[1] = pValue[0];
	temp[2] = pValue[1];

	return I2C_Write(I2C_GAS, MAX17048_ADDR, temp, 3) == HAL_OK;
}

/*
 * @brief      Read the current value of battery charge.
 * @param[out] Voltage The value of battery charge in mV.
 * @return 	   The success of the operation.
 */
uint8_t MAX17048_Read_Voltage(uint16_t* Voltage) {
	uint8_t result = 1;
	uint8_t buffer[2];
	uint16_t temp;

	result &= MAX17048_ReadReg(MAX17048_REG_VCELL, buffer, 2);
	temp = (((uint16_t) buffer[0]) << 8) | (uint16_t) buffer[1];
	*Voltage = (uint16_t) (((float) temp / 16) * 1.25);

	return result;
}

/*
 * @brief      Read the current battery's State of Charge (SOC).
 * @param[out] Soc The value of battery SOC in %. If the sensor is stuck, this
 * 				   value is set to -1 and an attempt to reset the sensor is
 * 				   automatically carried out.
 * @return 	   The success of the operation.
 */
uint8_t MAX17048_Read_SOC(uint8_t* Soc) {
	uint8_t result = 1;
	uint8_t buffer[2];

	result &= MAX17048_ReadReg(MAX17048_REG_SOC, buffer, 2);

	if (result) {
		if ((buffer[0] == 0 && buffer[1] == 0) || buffer[0] > 110) {
			*Soc = 0;

			// If the gas gauge is stuck, reset the IC
			uint16_t b = 0x5400;
			MAX17048_WriteReg(MAX17048_REG_CMD, b);
			result = 0;
		} else
			*Soc = buffer[0];
	} else
		*Soc = -1;

	return result;
}
