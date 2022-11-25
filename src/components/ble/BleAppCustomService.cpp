#include "components/ble/BleAppCustomService.h"
#include "components/heartrate/HeartRateController.h"
#include "systemtask/SystemTask.h"
#include <nrf_log.h>

using namespace Pinetime::Controllers;

namespace 
{
    constexpr ble_uuid128_t CharUuid(uint8_t x, uint8_t y)
    {
        return ble_uuid128_t 
        {
            .u = {.type = BLE_UUID_TYPE_128},
            .value = {0x97,0xb4,0x1c,0xac,0x12,0x10,0x9e,0x81,0x34,0x4a,0x03,0x88,x,y,0xb5,0x98}
        };
    }
    constexpr ble_uuid128_t appCustomServiceUuid {CharUuid(0x7e,0x4e)};
    constexpr ble_uuid128_t rollValueUuid {CharUuid(0x00,0x01)};
    constexpr ble_uuid128_t pitchValueUuid {CharUuid(0x02,0x03)};
    constexpr ble_uuid128_t yawValueUuid {CharUuid(0x04,0x05)};

    int BleAppCustomServiceCallback(uint16_t conn_handle, uint16_t attr_handle, struct ble_gatt_access_ctxt* ctxt, void* arg)
    {
        auto* bleAppCustomService = static_cast<BleAppCustomService*>(arg);
        return bleAppCustomService->OnAppCustomServiceRequest(conn_handle,
                                                    attr_handle, 
                                                    ctxt);
    }
}

BleAppCustomService::BleAppCustomService() 
: characteristicDefinition 
    {
        {
            .uuid = &rollValueUuid.u,
            .access_cb = BleAppCustomServiceCallback,
            .arg = this,
            .flags = BLE_GATT_CHR_F_READ,
            .val_handle = &rollValueHandle,
        },
        {
            .uuid = &pitchValueUuid.u,
            .access_cb = BleAppCustomServiceCallback,
            .arg = this,
            .flags = BLE_GATT_CHR_F_READ,
            .val_handle = &pitchValueHandle,

        },
        {
            .uuid = &yawValueUuid.u,
            .access_cb = BleAppCustomServiceCallback,
            .arg = this,
            .flags = BLE_GATT_CHR_F_READ,
            .val_handle = &yawValueHandle,
        },
        {0}
    },
    serviceDefinition 
    {
      {/* Device Information Service */
       .type = BLE_GATT_SVC_TYPE_PRIMARY,
       .uuid = &appCustomServiceUuid.u,
       .characteristics = characteristicDefinition
      },
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

int Pinetime::Controllers::BleAppCustomService::OnAppCustomServiceRequest(uint16_t connectionHandle, 
                                            uint16_t attributeHandle, 
                                            ble_gatt_access_ctxt* context)
{
    uint16_t buffer[6];
    if(attributeHandle=rollValueHandle)
    {
        NRF_LOG_INFO("roll value : handle = %d", rollValueHandle);
        buffer[0] = 1;
        int res = os_mbuf_append(context->om, buffer, 2);
        return (res == 0) ? 0 : BLE_ATT_ERR_INSUFFICIENT_RES;
    }

    if(attributeHandle=pitchValueHandle)
    {
        NRF_LOG_INFO("pitch value : handle = %d", pitchValueHandle);
        buffer[1] = 2;
        int res = os_mbuf_append(context->om, &buffer[2], 2);
        return (res == 0) ? 0 : BLE_ATT_ERR_INSUFFICIENT_RES;
    }

    if(attributeHandle = yawValueHandle)
    {
        NRF_LOG_INFO("yaw value : handle = %d", yawValueHandle);
        buffer[3] = 3;
        int res = os_mbuf_append(context->om, &buffer[4],2);
        return (res == 0) ? 0 : BLE_ATT_ERR_INSUFFICIENT_RES;
    }
    return 0;
}