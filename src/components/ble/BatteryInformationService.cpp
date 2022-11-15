#include "components/ble/BatteryInformationService.h"
#include <nrf_log.h>
#include "components/battery/BatteryController.h"

using namespace Pinetime::Controllers;

constexpr ble_uuid16_t BatteryInformationService::batteryInformationServiceUuid;
constexpr ble_uuid16_t BatteryInformationService::batteryLevelUuid;
constexpr ble_uuid128_t BatteryInformationService::batteryVoltageUuid;
constexpr ble_uuid128_t BatteryInformationService::extBatteryValueUuid;

int BatteryInformationServiceCallback(uint16_t conn_handle, uint16_t attr_handle, struct ble_gatt_access_ctxt* ctxt, void* arg) {
  auto* batteryInformationService = static_cast<BatteryInformationService*>(arg);
  return batteryInformationService->OnBatteryServiceRequested(conn_handle, attr_handle, ctxt);
}

BatteryInformationService::BatteryInformationService(Controllers::Battery& batteryController)
  : batteryController {batteryController},
    characteristicDefinition {{
                               .uuid = &batteryLevelUuid.u,
                               .access_cb = BatteryInformationServiceCallback,
                               .arg = this,
                               .flags = BLE_GATT_CHR_F_READ | BLE_GATT_CHR_F_NOTIFY,
                               .val_handle = &batteryLevelHandle
                               },
                               //add new battery voltage characteristic
                               {.uuid = &batteryVoltageUuid.u,
                               .access_cb = BatteryInformationServiceCallback,
                               .arg = this,
                               .flags = BLE_GATT_CHR_F_READ | BLE_GATT_CHR_F_NOTIFY,
                               .val_handle = &batteryVoltageHandle
                               },
                               //add new ext battery value characteristic
                               {.uuid = &extBatteryValueUuid.u,
                               .access_cb = BatteryInformationServiceCallback,
                               .arg = this,
                               .flags = BLE_GATT_CHR_F_READ | BLE_GATT_CHR_F_NOTIFY,
                               .val_handle = &extBatteryValueHandle
                               },
                              {0}
                              },
    serviceDefinition {
      {/* Device Information Service */
       .type = BLE_GATT_SVC_TYPE_PRIMARY,
       .uuid = &batteryInformationServiceUuid.u,
       .characteristics = characteristicDefinition},
      {0},
    } {
}


void BatteryInformationService::Init() {
  int res = 0;
  res = ble_gatts_count_cfg(serviceDefinition);
  ASSERT(res == 0);

  res = ble_gatts_add_svcs(serviceDefinition);
  ASSERT(res == 0);
}

int BatteryInformationService::OnBatteryServiceRequested(uint16_t connectionHandle,
                                                         uint16_t attributeHandle,
                                                         ble_gatt_access_ctxt* context) {
  if (attributeHandle == batteryLevelHandle) {
    NRF_LOG_INFO("BATTERY : handle = %d", batteryLevelHandle);
    uint8_t batteryValue = batteryController.PercentRemaining();
    int res = os_mbuf_append(context->om, &batteryValue, 1);
    return (res == 0) ? 0 : BLE_ATT_ERR_INSUFFICIENT_RES;
  }
  if(attributeHandle == batteryVoltageHandle)
  {
    NRF_LOG_INFO("BATTERY : handle = %d", batteryVoltageHandle);
    uint16_t batteryVoltageValue = batteryController.readBatteryVoltage();
    int res = os_mbuf_append(context->om, &batteryVoltageValue, sizeof(batteryVoltageValue));
    return (res == 0) ? 0 : BLE_ATT_ERR_INSUFFICIENT_RES;
  }
  if(attributeHandle == extBatteryValueHandle)
  {
    NRF_LOG_INFO("BATTERY : handle = %d", extBatteryValueHandle);
    uint16_t extBatteryValue = 255;
    int res = os_mbuf_append(context->om, &extBatteryValue, sizeof(extBatteryValue));
    return (res == 0) ? 0 : BLE_ATT_ERR_INSUFFICIENT_RES;
  }
  return 0;
}
void BatteryInformationService::NotifyBatteryLevel(uint16_t connectionHandle, uint8_t level) {
  auto* om = ble_hs_mbuf_from_flat(&level, 1);
  ble_gattc_notify_custom(connectionHandle, batteryLevelHandle, om);
}
void BatteryInformationService::NotifyBatteryVoltage(uint16_t connectionHandle, uint8_t voltage) 
{
  auto* om = ble_hs_mbuf_from_flat(&voltage, 1);
  ble_gattc_notify_custom(connectionHandle, batteryVoltageHandle, om);
}
