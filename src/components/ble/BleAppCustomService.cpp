#include "components/ble/BleAppCustomService.h"
#include "components/heartrate/HeartRateController.h"
#include "systemtask/SystemTask.h"
#include <nrf_log.h>

using namespace Pinetime::Controllers;

constexpr ble_uuid128_t BleAppCustomService::bleAppCustomServiceUuid;
constexpr ble_uuid16_t BleAppCustomService::customUuid;

int BleAppCustomServiceCallback(uint16_t conn_handle, uint16_t attr_handle, struct ble_gatt_access_ctxt* ctxt, void* arg)
{
    auto* bleAppCustomService = static_cast<BleAppCustomService*>(arg);
    return bleAppCustomService->OnAppCustomServiceRequest(conn_handle,
                                                attr_handle, 
                                                ctxt);
}

BleAppCustomService::BleAppCustomService() 
: characteristicDefinition 
    {
        {
            .uuid = &customUuid.u,
            .access_cb = BleAppCustomServiceCallback,
            .arg = this,
            .flags = BLE_GATT_CHR_F_READ,
        },
        {0}
    },
    serviceDefinition 
    {
      {/* Device Information Service */
       .type = BLE_GATT_SVC_TYPE_PRIMARY,
       .uuid = &bleAppCustomServiceUuid.u,
       .characteristics = characteristicDefinition},
      {0},
    } 
    {

    }

void BleAppCustomService::Init() 
{
  int res = 0;
  res = ble_gatts_count_cfg(serviceDefinition);
  ASSERT(res == 0);

  res = ble_gatts_add_svcs(serviceDefinition);
  ASSERT(res == 0);
}

int BleAppCustomService::OnAppCustomServiceRequest(uint16_t connectionHandle, 
                                            uint16_t attributeHandle, 
                                            ble_gatt_access_ctxt* context)
{
    const char* str;
    switch (ble_uuid_u16(context->chr->uuid)) 
    {
        case customId:
        str = customName; 
            break;
        default:
        return BLE_ATT_ERR_UNLIKELY;
    }
    int res = os_mbuf_append(context->om, str, strlen(str));
    return (res == 0) ? 0 : BLE_ATT_ERR_INSUFFICIENT_RES;

}