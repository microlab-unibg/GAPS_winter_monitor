/**
 * @file    BLE.c
 * @author  Daniele Comotti - daniele.comotti@unibg.it daniele.comotti@221e.it
 * @version V1.0 alpha
 * @date    07 November 2016
 * @brief   BLE implementation file
 * @details Edit this file at your own risk
 * Universit� degli studi di Bergamo, Microelectronics Laboratory - 221e srl
 */

#include "stdio.h"
#include "string.h"
#include "bluenrg_utils.h"
#include "stm32_bluenrg_ble.h"	// BlueNRG HAL
#include "hal_types.h"
#include "hci.h"
#include "bluenrg_aci.h"
#include "gp_timer.h"
#include "hal.h"
#include "osal.h"
#include "hci_const.h"
#include "bluenrg_gap.h"
#include "ble_status.h"
#include "sm.h"
#include "hci_le.h"
#include "bluenrg_7_2_c.h"
#include "../GattDB/gatt_db.h"
#include "app.h"
#include "ble.h"

static uint16_t connection_handle = 0;

static BLE_TypeDef BLE_config;

static uint8_t set_connectable = 1;

static uint8_t hw_version;
static uint16_t fw_version;

static uint16_t service_handle;
static uint16_t devname_char_handle;
static uint16_t appearance_char_handle;

#define IGNORE_STATUS(x) ((void)0)
#define CHECK_STATUS_RETURN(x) if (x != BLE_STATUS_SUCCESS) return
#define CHECK_STATUS_RETVAL(x) if (x != BLE_STATUS_SUCCESS) return x
#define CHECK_STATUS_RETBOOL(x) if (x != BLE_STATUS_SUCCESS) return 0

uint8_t BLE_Init(void) {
	uint8_t result = 1;

	// Init the Bluetooth Communication
	HCI_Init();
	HAL_Delay(10);
	HAL_GPIO_WritePin(BLE_RST_GPIO_Port, BLE_RST_Pin, GPIO_PIN_SET);
	HAL_NVIC_SetPriority(BNRG_SPI_EXTI_IRQn, BNRG_SPI_EXTI_IRQn_Priority, 0);
	HAL_NVIC_EnableIRQ(BNRG_SPI_EXTI_IRQn);
	strcpy(BLE_config.name, "Winter_GAPS");		//FIXME: Adjust device name

	// Add Services to BTLE
	HAL_Delay(10);
	BlueNRG_RST(); // Reset
	HAL_Delay(10);
	getBlueNRGVersion(&hw_version, &fw_version);
	if (0 < fw_version && fw_version < 0x723) {
		int ret = program_device(fw_image, sizeof fw_image);
		result &= (ret == BLE_STATUS_SUCCESS);
	}

	BLE_Services_Init();

	uint8_t temp[6];
	aci_hal_read_config_data(0x80, 6, temp, BLE_config.bd_address);

	//Init
	HAL_NVIC_SetPriority(BLE_JOB_IRQn, BLE_JOB_IRQn_Priority, 0);
	HAL_NVIC_EnableIRQ(BLE_JOB_IRQn);

	BLE_config.state = 1;
	set_connectable = 1;
	return result;
}

uint8_t BLE_Services_Init(void) {
	tBleStatus status;

	// Init the Bluetooth
	uint8_t bdaddr[] = { 0x89, 0xD3, 0xF5, 0xE1, 0x81, 0x02 };
	status = aci_hal_write_config_data(CONFIG_DATA_PUBADDR_OFFSET,
	CONFIG_DATA_PUBADDR_LEN, bdaddr);
	IGNORE_STATUS(status);

	// GATT Initialization
	status = aci_gatt_init();
	CHECK_STATUS_RETBOOL(status);
	status = aci_gap_init(GAP_PERIPHERAL_ROLE, 0x01, strlen(BLE_config.name),
			&service_handle, &devname_char_handle, &appearance_char_handle);
	CHECK_STATUS_RETBOOL(status);

	// Set Authorization Requirement
	status = aci_gap_set_auth_requirement(MITM_PROTECTION_REQUIRED,
	OOB_AUTH_DATA_ABSENT, NULL, 7, 16, USE_FIXED_PIN_FOR_PAIRING, 135712,
	BONDING);
	CHECK_STATUS_RETBOOL(status);

	status = Add_BLE_Service_DeviceInformation();
	CHECK_STATUS_RETBOOL(status);

	status = Add_BLE_Service_Battery();
	CHECK_STATUS_RETBOOL(status);

	status = Add_BLE_Service_EnvironmentalSensing();
	CHECK_STATUS_RETBOOL(status);

	status = Add_BLE_Service_Log();
	CHECK_STATUS_RETBOOL(status);

	status = Add_BLE_Service_Communication();
	CHECK_STATUS_RETBOOL(status);

	// Output power
	status = aci_hal_set_tx_power_level(1, 7); // +8dBm
	CHECK_STATUS_RETBOOL(status);

	return 1;
}

void BLE_SetConnectable(void) {
	char prefixed_name[BLE_NAME_LEN + 1] = { AD_TYPE_SHORTENED_LOCAL_NAME };
	strncpy(prefixed_name + 1, BLE_config.name, BLE_NAME_LEN);

	tBleStatus status;

	status = hci_le_set_scan_resp_data(0, NULL);
	CHECK_STATUS_RETURN(status);

	// Advertising period multiple of 0.625 ms
	status = aci_gap_set_discoverable(ADV_IND, SCAN_P, SCAN_L,
	STATIC_RANDOM_ADDR, NO_WHITE_LIST_USE, strlen(prefixed_name), prefixed_name,
			0, NULL, CONN_P1, CONN_P2);
	CHECK_STATUS_RETURN(status);

	HAL_Delay(10);

	// Change device name
	status = aci_gatt_update_char_value(service_handle, devname_char_handle, 0,
			strlen(BLE_config.name), BLE_config.name);
	CHECK_STATUS_RETURN(status);
}

void GAP_ConnectionComplete_CB(uint8_t addr[6], uint16_t handle) {
	connection_handle = handle;
	BLE_config.connected = 1;
	App_Handle_BTConnected();
//	MasterTrusted = false;
//	aci_gap_slave_security_request(handle, BONDING, MITM_PROTECTION_REQUIRED);
//	updateLastActivityTime(); TODO
}

void GAP_DisconnectionComplete_CB(void) {
//	updateLastActivityTime(); TODO
	BLE_config.connected = 0;
	set_connectable = 1;
	App_Handle_BTDisconnected();
}

void HCI_Event_CB(void *pckt) {
	hci_uart_pckt *hci_pckt = pckt;
	if (hci_pckt->type != HCI_EVENT_PKT) return;

	hci_event_pckt *event_pckt = (hci_event_pckt*) hci_pckt->data;
	switch (event_pckt->evt) {
		case EVT_DISCONN_COMPLETE:
			//  vt_disconn_complete *evt = (void*)event_pckt->data;
			GAP_DisconnectionComplete_CB();
			break;
		case EVT_LE_META_EVENT: {
			evt_le_meta_event *evt = (void*) event_pckt->data;
			switch (evt->subevent) {
				case EVT_LE_CONN_COMPLETE: {
					evt_le_connection_complete *cc = (void*) evt->data;
					GAP_ConnectionComplete_CB(cc->peer_bdaddr, cc->handle);
					//  if (memcmp(cc->peer_bdaddr, bioMX.accessory_bd, 6) == 0) {
					//      GAP_ConnectionComplete_CB(cc->peer_bdaddr, cc->handle);
					//  } else {
					//      for (int i = 0; i < 10; ++i) {
					//          HAL_Delay(25);
					//          if (aci_gap_terminate(cc->handle, HCI_REJECTED_SECURITY) == 0x00)
					//              break;
					//      }
					//  }
					break;
				}
			}
			break;
		}
		case EVT_VENDOR: {
			evt_blue_aci *blue_evt = (void*) event_pckt->data;
			switch (blue_evt->ecode) {
				case EVT_BLUE_GATT_READ_PERMIT_REQ:
					Read_Request_CB((void*) blue_evt->data);
					break;
				case EVT_BLUE_GATT_ATTRIBUTE_MODIFIED:
					Attribute_Modified_CB((void*) blue_evt->data);
					break;
					//  case EVT_BLUE_GAP_PAIRING_CMPLT: {
					//      evt_gap_pairing_cmplt *pairing = (void*)blue_evt->data;
					//      if (pairing->status == SM_PAIRING_SUCCESS)
					//          MasterTrusted = true;
					//      else
					//          aci_gap_terminate(connection_handle, HCI_REJECTED_SECURITY); TODO
					//  }
					//      break;
					//  case EVT_BLUE_GAP_BOND_LOST:
					//      aci_gap_allow_rebond(connection_handle);
					//		break;
				default:
					break;
			}
			break;
		}
	}
}

void BLE_DoJob(void) {
	HCI_Process();

	if (set_connectable) {
		BLE_SetConnectable();
		set_connectable = 0;
	}
}

uint8_t BLE_SetName(uint32_t *arg) {
	if (arg[0] == 0xFFFFFFFF) return 0;

	int i = 0;
	uint8_t tmp = arg[0] & 0xFF;
	while (i < BLE_NAME_LEN - 1 && tmp != '\0') {
		BLE_config.name[i] = tmp;
		++i;
		tmp = arg[i / 4] >> ((i % 4) * 8) & 0xFF;
	}
	BLE_config.name[i] = '\0';

	return 1;
}

void BLE_SetIRQs(uint8_t arg) {
	if (arg && BLE_config.state) {
		HAL_NVIC_EnableIRQ(BLE_JOB_IRQn);
		HAL_NVIC_EnableIRQ(BNRG_SPI_EXTI_IRQn);
	} else {
		HAL_NVIC_DisableIRQ(BLE_JOB_IRQn);
		HAL_NVIC_DisableIRQ(BNRG_SPI_EXTI_IRQn);
	}
}

void BLE_DeInit(void) {
	BLE_SetIRQs(0);
	HAL_GPIO_WritePin(BLE_RST_GPIO_Port, BLE_RST_Pin, GPIO_PIN_RESET);
	BLE_config.state = 0;
}

uint8_t BLE_IsConnected(void) {
	return BLE_config.connected;
}

uint8_t BLE_GetState(void) {
	return BLE_config.state;
}

void BLE_GetBDAddress(uint8_t *bd) {
	// BD address is stored in reverse mode
	for (int i = 0; i < 6; ++i)
		bd[i] = BLE_config.bd_address[i];
}

void BLE_Refresh(void) {
	// Refreshing BLE by resetting it
	BLE_DeInit();
	BLE_Init();
}
