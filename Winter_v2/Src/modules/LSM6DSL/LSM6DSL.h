/**
 ******************************************************************************
 * @file           : LSM6DSL.h
 * @brief          : Header file of LSM6DSL driver
 * @author		   : Patrick Locatelli (patrick.locatelli@unibg.it)
 * @version		   : 1.0
 * @date    	   : 13 Feb 2020
 ******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef MODULES_LSM6DSL_LSM6DSL_H_
#define MODULES_LSM6DSL_LSM6DSL_H_

/* Includes ------------------------------------------------------------------*/
#include "stdint.h"

/* Private defines -----------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
#define LSM6DSL_ADDRESS 		0xD6

#define LSM6DSL_ODR_SHIFT		4
typedef enum {
	LSM6DSL_ODR_POWER_DOWN = 0x00,
	LSM6DSL_ODR_12dot5Hz = 0x01 << LSM6DSL_ODR_SHIFT,
	LSM6DSL_ODR_26Hz = 0x02 << LSM6DSL_ODR_SHIFT,
	LSM6DSL_ODR_52Hz = 0x03 << LSM6DSL_ODR_SHIFT,
	LSM6DSL_ODR_104Hz = 0x04 << LSM6DSL_ODR_SHIFT,
	LSM6DSL_ODR_208Hz = 0x05 << LSM6DSL_ODR_SHIFT,
	LSM6DSL_ODR_416Hz = 0x06 << LSM6DSL_ODR_SHIFT,
	LSM6DSL_ODR_833Hz = 0x07 << LSM6DSL_ODR_SHIFT,
	LSM6DSL_ODR_1660Hz = 0x08 << LSM6DSL_ODR_SHIFT,
	LSM6DSL_ODR_3330Hz = 0x09 << LSM6DSL_ODR_SHIFT,
	LSM6DSL_ODR_6660Hz = 0x0A << LSM6DSL_ODR_SHIFT,
} LSM6DSL_ODR;

/* --- CTRL1_XL --------------------------------------------------------------*/
#define LSM6DSL_REG_CTRL1_XL		0x10

#define LSM6DSL_REG_CTRL1_FS_SHIFT	2
typedef enum {
	LSM6DSL_FS_XL_2g = 0x00 << LSM6DSL_REG_CTRL1_FS_SHIFT,
	LSM6DSL_FS_XL_16g = 0x01 << LSM6DSL_REG_CTRL1_FS_SHIFT,
	LSM6DSL_FS_XL_4g = 0x02 << LSM6DSL_REG_CTRL1_FS_SHIFT,
	LSM6DSL_FS_XL_8g = 0x03 << LSM6DSL_REG_CTRL1_FS_SHIFT,
} LSM6DSL_FS_XL;

/* --- CTRL2_G ---------------------------------------------------------------*/
#define LSM6DSL_REG_CTRL2_G				0x11

#define LSM6DSL_REG_CTRL2_FS_125_SHIFT	1
#define LSM6DSL_REG_CTRL2_FS_G_SHIFT	2
typedef enum {
	LSM6DSL_FS_G_125dps = 0x01 << LSM6DSL_REG_CTRL2_FS_125_SHIFT,
	LSM6DSL_FS_G_250dps = 0x00 << LSM6DSL_REG_CTRL2_FS_G_SHIFT,
	LSM6DSL_FS_G_500dps = 0x01 << LSM6DSL_REG_CTRL2_FS_G_SHIFT,
	LSM6DSL_FS_G_1000dps = 0x02 << LSM6DSL_REG_CTRL2_FS_G_SHIFT,
	LSM6DSL_FS_G_2000dps = 0x03 << LSM6DSL_REG_CTRL2_FS_G_SHIFT,
} LSM6DSL_FS_G;

/* --- CTRL3_C ---------------------------------------------------------------*/
#define LSM6DSL_REG_CTRL3_C			0x12

#define LSM6DSL_REG_CTRL3_BDU_SHIFT	6
typedef enum {
	LSM6DSL_BDU_CONTINUOUS = 0x00 | 0x04,
	LSM6DSL_BDU_BLOCKING = (0x01 << LSM6DSL_REG_CTRL3_BDU_SHIFT) | 0x04
} LSM6DSL_BDU;
// Comment: 0x04 -> IF_INC = 1

/* --- CTRL6_C ---------------------------------------------------------------*/
#define LSM6DSL_REG_CTRL6_C					0x15

#define LSM6DSL_REG_CTRL6_XL_HM_MODE_SHIFT	4
typedef enum {
	LSM6DSL_XL_HM_MODE_HIGH_PERFORMANCE = 0x00,
	LSM6DSL_XL_HM_MODE_LOW_NORMAL = 0x01 << LSM6DSL_REG_CTRL6_XL_HM_MODE_SHIFT
} LSM6DSL_XL_HM_MODE;

/* --- CTRL7_C ---------------------------------------------------------------*/
#define LSM6DSL_REG_CTRL7_C					0x16

#define LSM6DSL_REG_CTRL7_G_HM_MODE_SHIFT	7
typedef enum {
	LSM6DSL_G_HM_MODE_HIGH_PERFORMANCE = 0x00,
	LSM6DSL_G_HM_MODE_LOW_NORMAL = 0x01 << LSM6DSL_REG_CTRL7_G_HM_MODE_SHIFT
} LSM6DSL_G_HM_MODE;

/* --- TAP_CFG ---------------------------------------------------------------*/
#define LSM6DSL_REG_TAP_CFG						0x58

#define LSM6DSL_REG_TAP_CFG_INT_ENABLE_SHIFT	7
typedef enum {
	LSM6DSL_INTERRPUTS_DISABLED = 0x00,
	LSM6DSL_INTERRPUTS_ENABLED = (0x01 << LSM6DSL_REG_TAP_CFG_INT_ENABLE_SHIFT)
} LSM6DSL_INT_ENABLE;

#define LSM6DSL_REG_TAP_CFG_AXIS_ENABLED_SHIFT	1

typedef enum {
	LSM6DSL_LIR_DISABLED = 0x00,
	LSM6DSL_LIR_ENABLED = 0x01
} LSM6DSL_LIR_ENABLE;

/* --- TAP_THS_6D ------------------------------------------------------------*/
#define LSM6DSL_REG_TAP_THS_6D					0x59

/* --- INT_DUR2 --------------------------------------------------------------*/
#define LSM6DSL_REG_INT_DUR2					0x5A

#define LSM6DSL_REG_INT_DUR2_DUR_SHIFT			4
#define LSM6DSL_REG_INT_DUR2_QUIET_SHIFT		2
#define LSM6DSL_REG_INT_DUR2_SHOCK_SHIFT		0

/* --- WAKE_UP_THS -----------------------------------------------------------*/
#define LSM6DSL_REG_WAKE_UP_THS					0x5B

#define LSM6DSL_REG_WAKE_UP_THS_SDTAP_SHIFT		7
typedef enum {
	LSM6DSL_SDTAP_DISABLED = 0x00,
	LSM6DSL_SDTAP_ENABLED = 0x01 << LSM6DSL_REG_WAKE_UP_THS_SDTAP_SHIFT
} LSM6DSL_SDTAP;

/* --- MD1_CFG ---------------------------------------------------------------*/
#define LSM6DSL_REG_MD1_CFG						0x5E

#define LSM6DSL_REG_MD1_CFG_INT1_DTAP_SHIFT		3
typedef enum {
	LSM6DSL_INT1_DTAP_DISABLED = 0x00,
	LSM6DSL_INT1_DTAP_ENABLED = 0x01 << LSM6DSL_REG_MD1_CFG_INT1_DTAP_SHIFT
} LSM6DSL_INT1_DTAP;

/* --- Other registers -------------------------------------------------------*/
#define LSM6DSL_REG_WHO_AM_I		0x0F
#define LSM6DSL_WHO_AM_I_CONTENT	0x6A

#define LSM6DSL_REG_TAP_SRC		0x1C

#define LSM6DSL_REG_OUTX_L_G	0x22
#define LSM6DSL_REG_OUTX_H_G	0x23
#define LSM6DSL_REG_OUTY_L_G	0x24
#define LSM6DSL_REG_OUTY_H_G	0x25
#define LSM6DSL_REG_OUTZ_L_G	0x26
#define LSM6DSL_REG_OUTZ_H_G	0x27

#define LSM6DSL_REG_OUTX_L_XL	0x28
#define LSM6DSL_REG_OUTX_H_XL	0x29
#define LSM6DSL_REG_OUTY_L_XL	0x2A
#define LSM6DSL_REG_OUTY_H_XL	0x2B
#define LSM6DSL_REG_OUTZ_L_XL	0x2C
#define LSM6DSL_REG_OUTZ_H_XL	0x2D

/* --- Base sensitivities ----------------------------------------------------*/
#define LSM6DSL_AXL_SENSITIVITY_2g	0.061
#define LSM6DSL_AXL_SENSITIVITY_4g	0.122
#define LSM6DSL_AXL_SENSITIVITY_8g	0.244
#define LSM6DSL_AXL_SENSITIVITY_16g	0.488

#define LSM6DSL_GYRO_SENSITIVITY_125dps		4.375
#define LSM6DSL_GYRO_SENSITIVITY_250dps		8.75
#define LSM6DSL_GYRO_SENSITIVITY_500dps		17.5
#define LSM6DSL_GYRO_SENSITIVITY_1000dps	35
#define LSM6DSL_GYRO_SENSITIVITY_2000dps	70

/* --- Structure definition ---------------------------------------------------*/
typedef struct {
	LSM6DSL_ODR axl_odr;
	LSM6DSL_FS_XL axl_fs;
	LSM6DSL_XL_HM_MODE axl_mode;
	float axl_sensitivity;
	LSM6DSL_ODR gyro_odr;
	LSM6DSL_FS_G gyro_fs;
	LSM6DSL_G_HM_MODE gyro_mode;
	float gyro_sensitivity;
	LSM6DSL_BDU bdu;

	// Double-tap variables
	LSM6DSL_INT_ENABLE int_enable;
	uint8_t int_enabledAxis;
	uint8_t int_lir;
	uint8_t int_tapThs;
	uint8_t int_tapShock;
	uint8_t int_tapQuiet;
	uint8_t int_tapDur;
	LSM6DSL_SDTAP int_SDTapEnable;
	LSM6DSL_INT1_DTAP int1_dt;
} LSM6DSL_Init_t;

/* Exported functions prototypes ---------------------------------------------*/
uint8_t LSM6DSL_Read_Reg(uint8_t Register, uint8_t* ReadByte, uint16_t Size);
uint8_t LSM6DSL_Write_Reg(uint8_t Register, uint8_t Value);
uint8_t LSM6DSL_Config(LSM6DSL_Init_t* pNewConfig);
uint8_t LSM6DSL_Read_Axl(uint16_t* pAxl_digits, float* pAxl);
uint8_t LSM6DSL_Read_Gyro(uint16_t* pGyro_digits, float* pGyro);
uint8_t LSM6DSL_PowerDown_Axl(uint8_t arg);
uint8_t LSM6DSL_PowerDown_Gyro(uint8_t arg);
LSM6DSL_ODR LSM6DSL_Get_Axl_ODR(void);
uint8_t LSM6DSL_Set_Axl_ODR(LSM6DSL_ODR NewODR);
LSM6DSL_ODR LSM6DSL_Get_Axl_ODR(void);
uint8_t LSM6DSL_Set_Gyro_ODR(LSM6DSL_ODR NewODR);
uint8_t LSM6DSL_Enable_DoubleTapInterrupt(uint8_t arg);
uint8_t LSM6DSL_Reset_DoubleTapInterrupt(void);
uint8_t LSM6DSL_Check_WhoAmI(void);

#endif /* MODULES_LSM6DSL_LSM6DSL_H_ */
