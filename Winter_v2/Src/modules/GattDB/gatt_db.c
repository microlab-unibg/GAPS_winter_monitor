#include "app.h"
#include "../BLE/ble.h"
#include "bluenrg_aci.h"
#include "bluenrg_gap.h"
#include "bluenrg_gatt_server.h"
#include "gatt_db.h"
#include "hal.h"
#include "hal_types.h"
#include "main.h"
#include "stdbool.h"
#include "string.h"
#include "comm.h"
#include <assert.h>
#include <stdio.h>

/////////////////////////
BLE_Config ble_config;

////////////////////////

#ifdef NDEBUG
#define PRINTF(...)
#else
#define PRINTF(...) printf(__VA_ARGS__)
#endif

#define CHECK_STATUS_RETURN(x) if (x != BLE_STATUS_SUCCESS) return
#define CHECK_STATUS_RETVAL(x) if (x != BLE_STATUS_SUCCESS) return x

static uint8_t const uuid_user_description[2] = { 0x01, 0x29 };

static uint16_t handle_batt_serv;
static uint16_t handle_batt_level;

static uint16_t handle_ess; //Environmental Sensing Service
static uint16_t handle_temperature;
static uint16_t handle_humidity;
static uint16_t handle_pressure;

static uint16_t handle_log_serv;
static uint16_t handle_log_status;

static uint16_t handle_comm_serv; //servizio di comunicazione comandi
static uint16_t handle_comm_value;

tBleStatus Add_BLE_Service_DeviceInformation(void) {
	// TODO Consider adding a "Serial Number String" (0x2925)

	uint8_t const max_attr_records = 7;
	// Device Information
	//   Manufacturer Name String + Value
	//   Hardware Revision String + Value
	//   Firmware Revision String + Value

	uint8_t const uuid_serv[2] = { 0x0A, 0x18 };
	uint8_t const uuid_manufacturer[2] = { 0x29, 0x2A };
	uint8_t const uuid_hwrevision[2] = { 0x27, 0x2A };
	uint8_t const uuid_fwrevision[2] = { 0x26, 0x2A };

	uint16_t handle_serv;
	uint16_t handle_manufacturer;
	uint16_t handle_hwrevision;
	uint16_t handle_fwrevision;

	char const manufacturer[] = "MicroLab";
	uint8_t const manufacturer_len = strlen(manufacturer);

	char const hwrevision[] = "1.0";
	uint8_t const hwrevision_len = strlen(hwrevision);

	char const fwrevision[] = "1.0.0";
	uint8_t const fwrevision_len = strlen(fwrevision);

	tBleStatus status;

	status = aci_gatt_add_serv(UUID_TYPE_16, uuid_serv, PRIMARY_SERVICE,
			max_attr_records, &handle_serv);
	CHECK_STATUS_RETVAL(status);

	status = aci_gatt_add_char(handle_serv, UUID_TYPE_16, uuid_manufacturer,
			manufacturer_len,
			CHAR_PROP_READ, ATTR_PERMISSION_NONE, GATT_DONT_NOTIFY_EVENTS, 16,
			CHAR_VALUE_LEN_CONSTANT, &handle_manufacturer);

	CHECK_STATUS_RETVAL(status);

	status = aci_gatt_add_char(handle_serv, UUID_TYPE_16, uuid_hwrevision,
			hwrevision_len,
			CHAR_PROP_READ, ATTR_PERMISSION_NONE, GATT_DONT_NOTIFY_EVENTS, 16,
			CHAR_VALUE_LEN_CONSTANT, &handle_hwrevision);

	CHECK_STATUS_RETVAL(status);

	status = aci_gatt_add_char(handle_serv, UUID_TYPE_16, uuid_fwrevision,
			fwrevision_len,
			CHAR_PROP_READ, ATTR_PERMISSION_NONE, GATT_DONT_NOTIFY_EVENTS, 16,
			CHAR_VALUE_LEN_CONSTANT, &handle_fwrevision);

	CHECK_STATUS_RETVAL(status);

	status = aci_gatt_update_char_value(handle_serv, handle_manufacturer, 0,
			manufacturer_len, manufacturer);
	CHECK_STATUS_RETVAL(status);
	status = aci_gatt_update_char_value(handle_serv, handle_hwrevision, 0,
			hwrevision_len, hwrevision);
	CHECK_STATUS_RETVAL(status);
	status = aci_gatt_update_char_value(handle_serv, handle_fwrevision, 0,
			fwrevision_len, fwrevision);
	CHECK_STATUS_RETVAL(status);

	return BLE_STATUS_SUCCESS;
}

tBleStatus Add_BLE_Service_Battery(void) {
	uint8_t const max_attr_records = 4;
	// Battery Service
	//   Battery Level + Value
	//     Client Characteristic Configuration

	uint8_t const uuid_batt_serv[2] = { 0x0F, 0x18 };
	uint8_t const uuid_batt_level[2] = { 0x19, 0x2A };

	tBleStatus status;

	status = aci_gatt_add_serv(UUID_TYPE_16, uuid_batt_serv, PRIMARY_SERVICE,
			max_attr_records, &handle_batt_serv);
	CHECK_STATUS_RETVAL(status);

	// TODO How should notification be implemented?
	status = aci_gatt_add_char(handle_batt_serv, UUID_TYPE_16, uuid_batt_level,
			1,
			CHAR_PROP_READ | CHAR_PROP_NOTIFY, ATTR_PERMISSION_NONE,
			GATT_NOTIFY_READ_REQ_AND_WAIT_FOR_APPL_RESP, 16,
			CHAR_VALUE_LEN_CONSTANT, &handle_batt_level);

	CHECK_STATUS_RETVAL(status);

	return BLE_STATUS_SUCCESS;
}

tBleStatus Add_BLE_Service_EnvironmentalSensing(void) {
	uint8_t const max_attr_records = 12;
	// Environmental Sensing Service
	// Temperature + Value
	// Humidity + Value
	// Pressure + Value

	uint8_t const uuid_ess[2] = { 0x1A, 0x18 };
	uint8_t const uuid_temperature[2] = { 0x6E, 0x2A };
	uint8_t const uuid_humidity[2] = { 0x6F, 0x2A };
	uint8_t const uuid_pressure[2] = { 0x6D, 0x2A };
	tBleStatus status;

	status = aci_gatt_add_serv(UUID_TYPE_16, uuid_ess, PRIMARY_SERVICE,
			max_attr_records, &handle_ess);
	CHECK_STATUS_RETVAL(status);

	// TODO How should notification be implemented?
	status = aci_gatt_add_char(handle_ess,
	UUID_TYPE_16, uuid_temperature, 2,
	CHAR_PROP_READ | CHAR_PROP_NOTIFY,
	ATTR_PERMISSION_NONE,
	GATT_NOTIFY_READ_REQ_AND_WAIT_FOR_APPL_RESP, 16,
	CHAR_VALUE_LEN_CONSTANT, &handle_temperature);

	status = aci_gatt_add_char(handle_ess, UUID_TYPE_16, uuid_humidity, 2,
	CHAR_PROP_READ | CHAR_PROP_NOTIFY, ATTR_PERMISSION_NONE,
	GATT_NOTIFY_READ_REQ_AND_WAIT_FOR_APPL_RESP, 16,
	CHAR_VALUE_LEN_CONSTANT, &handle_humidity);

	status = aci_gatt_add_char(handle_ess, UUID_TYPE_16, uuid_pressure, 4,
	CHAR_PROP_READ | CHAR_PROP_NOTIFY, ATTR_PERMISSION_NONE,
	GATT_NOTIFY_READ_REQ_AND_WAIT_FOR_APPL_RESP, 16,
	CHAR_VALUE_LEN_CONSTANT, &handle_pressure);

	CHECK_STATUS_RETVAL(status);

	return BLE_STATUS_SUCCESS;
}
tBleStatus Add_BLE_Service_Log(void) {
	uint8_t const max_attr_records = 4;
	// Custom Service (Log)
	//   Custom Characteristic (Log Status) + Value
	//     Characteristic User Description

	uint8_t const uuid_log_serv[16] = { 0xD1, 0x9F, 0x1B, 0x1F, 0x80, 0xF2,
			0x32, 0x9F, 0xE8, 0x11, 0x69, 0xE6, 0x94, 0xCC, 0xAB, 0x31, };

	uint8_t const uuid_log_status[16] = { 0xD1, 0x9F, 0x1B, 0x1F, 0x80, 0xF2,
			0x32, 0x9F, 0xE8, 0x11, 0x69, 0xE6, 0xBE, 0xD1, 0x8C, 0x6C, };

	uint16_t handle_log_status_userdesc;
	char const log_status_userdesc[] = "Log Status";
	uint8_t const log_status_userdesc_len = strlen(log_status_userdesc);

	tBleStatus status;

	status = aci_gatt_add_serv(UUID_TYPE_128, uuid_log_serv, PRIMARY_SERVICE,
			max_attr_records, &handle_log_serv);

	CHECK_STATUS_RETVAL(status);

	status = aci_gatt_add_char(handle_log_serv, UUID_TYPE_128, uuid_log_status,
			1,
			CHAR_PROP_READ | CHAR_PROP_WRITE, ATTR_PERMISSION_NONE,
			GATT_NOTIFY_READ_REQ_AND_WAIT_FOR_APPL_RESP
					| GATT_NOTIFY_ATTRIBUTE_WRITE, 16, CHAR_VALUE_LEN_CONSTANT,
			&handle_log_status);

	CHECK_STATUS_RETVAL(status);

	status = aci_gatt_add_char_desc(handle_log_serv, handle_log_status,
	UUID_TYPE_16, uuid_user_description, log_status_userdesc_len,
			log_status_userdesc_len, log_status_userdesc,
			ATTR_PERMISSION_NONE,
			ATTR_ACCESS_READ_ONLY,
			GATT_DONT_NOTIFY_EVENTS, 16, CHAR_VALUE_LEN_CONSTANT,
			&handle_log_status_userdesc);

	CHECK_STATUS_RETVAL(status);

	return BLE_STATUS_SUCCESS;
}

tBleStatus Add_BLE_Service_Communication(void) {
	uint8_t const max_attr_records = 4;
	// Communication Service
	//   Accelerometer + Value
	//     Client Characteristic Configuration

	uint8_t const uuid_comm_serv[16] = { 0x21, 0x45, 0xF7, 0xA0, 0x6B, 0x50,
			0x11, 0xE9, 0xB4, 0x75, 0x08, 0x00, 0x20, 0x0C, 0x9A, 0x66 };

	uint8_t const uuid_comm_value[16] = { 0xE1, 0xAB, 0x9E, 0x10, 0x71, 0x94,
			0x11, 0xE9, 0xB4, 0x75, 0x08, 0x00, 0x20, 0x0C, 0x9A, 0x66 };

	tBleStatus status;

	status = aci_gatt_add_serv(UUID_TYPE_128, uuid_comm_serv, PRIMARY_SERVICE,
			max_attr_records, &handle_comm_serv);
	CHECK_STATUS_RETVAL(status);

	status = aci_gatt_add_char(handle_comm_serv, UUID_TYPE_128, uuid_comm_value,
	BLE_CMD_MAX_LEN,
	CHAR_PROP_READ | CHAR_PROP_WRITE, ATTR_PERMISSION_NONE,
	GATT_NOTIFY_READ_REQ_AND_WAIT_FOR_APPL_RESP | GATT_NOTIFY_ATTRIBUTE_WRITE,
			16,
			CHAR_VALUE_LEN_CONSTANT, &handle_comm_value);

	CHECK_STATUS_RETVAL(status);

	return BLE_STATUS_SUCCESS;
}

void Read_Request_CB(evt_gatt_read_permit_req const *evt) {
	tBleStatus status;

	if (evt->attr_handle == handle_batt_level + 1) {
		// Battery level (SOC)
		uint8_t value;
		MAX17048_Read_SOC(&value);
		status = aci_gatt_update_char_value(handle_batt_serv, handle_batt_level,
				0, 1, &value);
	} else if (evt->attr_handle == handle_temperature + 1) {
		// Temperature (0.01 °C)
		float temperature;
		HTS221_Read_Temperature(&temperature);			// °C
		int16_t temp = (int16_t) (temperature * 100);	// from °C to 0.01 °C
		int8_t value[2];
		value[1] = (uint8_t) (temp & 0xFF);
		value[0] = (uint8_t) ((temp >> 8) & 0xFF);
		status = aci_gatt_update_char_value(handle_ess, handle_temperature, 0,
				2, value);
	} else if (evt->attr_handle == handle_humidity + 1) {
		// Humidity (0.01% rH)
		float humidity;
		HTS221_Read_Humidity(&humidity);				// %
		uint16_t hum = (uint16_t) (humidity * 100);		// from % to 0.01%
		uint8_t value[2];
		value[1] = (uint8_t) (hum & 0xFF);
		value[0] = (uint8_t) ((hum >> 8) & 0xFF);
		status = aci_gatt_update_char_value(handle_ess, handle_humidity, 0, 2,
				value);
	} else if (evt->attr_handle == handle_pressure + 1) {
		// Pressure (0.1 Pa)
		float pressure;
		LPS22HH_Read_Pressure(&pressure);				// hPa
		uint32_t press = (uint32_t) (pressure * 1000);	// from hPa to dPa
		uint8_t value[4];
		value[3] = (uint8_t) (press & 0xFF);
		value[2] = (uint8_t) ((press >> 8) & 0xFF);
		value[1] = (uint8_t) ((press >> 16) & 0xFF);
		value[0] = (uint8_t) ((press >> 24) & 0xFF);
		status = aci_gatt_update_char_value(handle_ess, handle_pressure, 0, 4,
				&value);
	} else if (evt->attr_handle == handle_log_status + 1) {
		// Log status
		uint8_t value = App_GetSystemState() == SYSTEM_STATE_LOG;
		status = aci_gatt_update_char_value(handle_log_serv, handle_log_status,
				0, 1, &value);
	} else if (evt->attr_handle == handle_comm_value + 1) {
		// Communication (?)
		uint8_t value = App_GetSystemState();
		status = aci_gatt_update_char_value(handle_comm_serv, handle_comm_value,
				0, 1, &value);
	}

	CHECK_STATUS_RETURN(status);
	status = aci_gatt_allow_read(evt->conn_handle);
	CHECK_STATUS_RETURN(status);
}

void Attribute_Modified_CB(evt_gatt_attr_modified const *evt) {

	if (evt->attr_handle == handle_comm_value + 1) {
		ble_config.updateChar = Evaluate_Command(evt->att_data,
				evt->data_length, ble_config.respValue, &(ble_config.respLen));

		if (ble_config.updateChar) {
			aci_gatt_update_char_value(handle_comm_serv, handle_comm_value, 0,
					ble_config.respLen, ble_config.respValue);
		}
	}
}
