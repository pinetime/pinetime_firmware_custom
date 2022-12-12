#include "components/ble/BleAppCustomService.h"
#include "components/heartrate/HeartRateController.h"
#include "components/motion/MotionController.h"
#include "systemtask/SystemTask.h"
#include <nrf_log.h>

using namespace Pinetime::Controllers;

constexpr ble_uuid16_t BleAppCustomService::appCustomServiceUuid;
constexpr ble_uuid16_t BleAppCustomService::rollValueUuid;
constexpr ble_uuid16_t BleAppCustomService::pitchValueUuid;
constexpr ble_uuid16_t BleAppCustomService::yawValueUuid;
constexpr ble_uuid16_t BleAppCustomService::heartBitUuid;
namespace 
{

    int BleAppCustomServiceCallback(uint16_t conn_handle, 
                                    uint16_t attr_handle, 
                                    struct ble_gatt_access_ctxt* ctxt, 
                                    void* arg)
    {
        auto* bleAppCustomService = static_cast<BleAppCustomService*>(arg);
        return bleAppCustomService->OnAppCustomServiceRequest(conn_handle,
                                                    attr_handle, 
                                                    ctxt);
    }
}

BleAppCustomService::BleAppCustomService(Pinetime::System::SystemTask& system, 
                                        Controllers::MotionController& motionController) 
:   system {system},
    motionController {motionController},
    characteristicDefinition 
    {
        {
            .uuid = &rollValueUuid.u,
            .access_cb = BleAppCustomServiceCallback,
            .arg = this,
            .flags = BLE_GATT_CHR_F_READ | BLE_GATT_CHR_F_NOTIFY,
            .val_handle = &rollValueHandle,
        },
        {
            .uuid = &pitchValueUuid.u,
            .access_cb = BleAppCustomServiceCallback,
            .arg = this,
            .flags = BLE_GATT_CHR_F_READ | BLE_GATT_CHR_F_NOTIFY,
            .val_handle = &pitchValueHandle,

        },
        {
            .uuid = &yawValueUuid.u,
            .access_cb = BleAppCustomServiceCallback,
            .arg = this,
            .flags = BLE_GATT_CHR_F_READ | BLE_GATT_CHR_F_NOTIFY,
            .val_handle = &yawValueHandle,
        },
        {
            .uuid = &heartBitUuid.u,
            .access_cb = BleAppCustomServiceCallback,
            .arg = this,
            .flags = BLE_GATT_CHR_F_READ | BLE_GATT_CHR_F_NOTIFY,
            .val_handle = &heartBitHandle,
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
        motionController._SetService(this);
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
    if(attributeHandle=rollValueHandle)
    {
        int16_t rollBuffer[1] = {motionController.X()};
        int res = os_mbuf_append(context->om, rollBuffer, 1 * sizeof(int16_t));
        return (res == 0) ? 0 : BLE_ATT_ERR_INSUFFICIENT_RES;
    }

    else if(attributeHandle=pitchValueHandle)
    {
        int16_t pitchBuffer[1]= {motionController.Y()};
        int res = os_mbuf_append(context->om, pitchBuffer, 1 * sizeof(int16_t));
        return (res == 0) ? 0 : BLE_ATT_ERR_INSUFFICIENT_RES;
    }

    else if(attributeHandle = yawValueHandle)
    {
        int16_t yawBuffer[1] = {motionController.Z()};
        int res = os_mbuf_append(context->om, yawBuffer, 1 * sizeof(int16_t));
        return (res == 0) ? 0 : BLE_ATT_ERR_INSUFFICIENT_RES;
    }
    return 0;
}

void BleAppCustomService::OnNewRollValues(int16_t x)
{
    if (!rollValuesNoficationEnabled)
    return;

    int16_t buffer[1] = {motionController.X()};
    auto* om = ble_hs_mbuf_from_flat(buffer, 1 * sizeof(int16_t));

    uint16_t connectionHandle = system.nimble().connHandle();

    if (connectionHandle == 0 || connectionHandle == BLE_HS_CONN_HANDLE_NONE) 
    {
        return;
    }

    ble_gattc_notify_custom(connectionHandle, rollValueHandle, om);
}

void BleAppCustomService::OnNewPitchValues(int16_t y)
{
    if (!pitchValuesNoficationEnabled)
    return;

    int16_t buffer[1] = {motionController.Y()};
    auto* om = ble_hs_mbuf_from_flat(buffer, 1 * sizeof(int16_t));

    uint16_t connectionHandle = system.nimble().connHandle();

    if (connectionHandle == 0 || connectionHandle == BLE_HS_CONN_HANDLE_NONE) 
    {
        return;
    }

    ble_gattc_notify_custom(connectionHandle, pitchValueHandle, om);
}

void BleAppCustomService::OnNewYawValues(int16_t z)
{
    if (!yawValuesNoficationEnabled)
    return;

    int16_t buffer[1] = {motionController.Z()};
    auto* om = ble_hs_mbuf_from_flat(buffer, 1 * sizeof(int16_t));

    uint16_t connectionHandle = system.nimble().connHandle();

    if (connectionHandle == 0 || connectionHandle == BLE_HS_CONN_HANDLE_NONE) 
    {
        return;
    }

    ble_gattc_notify_custom(connectionHandle, yawValueHandle, om);
}

void BleAppCustomService::SubscribeNotification(uint16_t connectionHandle, uint16_t attributeHandle) 
{
    if (attributeHandle == rollValueHandle)
    rollValuesNoficationEnabled = true;
    else if (attributeHandle == pitchValueHandle)
    pitchValuesNoficationEnabled = true;
    else if (attributeHandle == yawValueHandle)
    yawValuesNoficationEnabled = true;
}

void BleAppCustomService::UnsubscribeNotification(uint16_t connectionHandle, uint16_t attributeHandle) 
{
    if (attributeHandle == rollValueHandle)
    rollValuesNoficationEnabled = false;
    else if (attributeHandle == pitchValueHandle)
    pitchValuesNoficationEnabled = false;
    else if (attributeHandle == yawValueHandle)
    yawValuesNoficationEnabled = false;
}