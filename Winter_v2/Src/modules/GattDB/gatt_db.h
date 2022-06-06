#ifndef MODULES_GATTDB_GATTDB_H_
#define MODULES_GATTDB_GATTDB_H_

#include "ble_status.h"
#include "bluenrg_gatt_aci.h"

tBleStatus Add_BLE_Service_DeviceInformation(void);
tBleStatus Add_BLE_Service_Battery(void);
tBleStatus Add_BLE_Service_EnvironmentalSensing(void);
tBleStatus Add_BLE_Service_Log(void);
tBleStatus Add_BLE_Service_Communication(void);
void Read_Request_CB(evt_gatt_read_permit_req const *evt);
void Attribute_Modified_CB(evt_gatt_attr_modified const *evt);

#endif /* MODULES_GATTDB_GATTDB_H_ */
