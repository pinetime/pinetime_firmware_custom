#pragma once
#define min // workaround: nimble's min/max macros conflict with libstdc++
#define max
#include <host/ble_gap.h>
#include <atomic>
#include <cstdint>
#undef max
#undef min

namespace Pinetime 
{
    namespace System 
    {
        class SystemTask;
    }
    namespace Controllers
    {
        class MotionController;
        class BleAppCustomService
        {
            public:
                BleAppCustomService(Pinetime::System::SystemTask& system, Controllers::MotionController& motionController);
                void Init();
                int OnAppCustomServiceRequest(uint16_t connectionHandle, uint16_t attributeHandle, ble_gatt_access_ctxt* context);
                void SubscribeNotification(uint16_t connectionHandle, uint16_t attributeHandle);
                void UnsubscribeNotification(uint16_t connectionHandle, uint16_t attributeHandle);
                void OnNewRollValues(int16_t x);
                void OnNewPitchValues(int16_t y);
                void OnNewYawValues(int16_t z);


            private:
                Pinetime::System::SystemTask& system;

                static constexpr ble_uuid16_t appCustomServiceUuid
                {
                    .u {.type = BLE_UUID_TYPE_16}, 
                    .value {0x1111}
                };
                static constexpr ble_uuid16_t rollValueUuid 
                {
                    .u {.type = BLE_UUID_TYPE_16}, 
                    .value {0x1112}
                };
                static constexpr ble_uuid16_t pitchValueUuid 
                {
                    .u {.type = BLE_UUID_TYPE_16}, 
                    .value {0x1113}
                };
                static constexpr ble_uuid16_t yawValueUuid 
                {
                    .u {.type = BLE_UUID_TYPE_16}, 
                    .value {0x1114}
                };
                struct ble_gatt_chr_def characteristicDefinition[4];
                struct ble_gatt_svc_def serviceDefinition[2];
                Controllers::MotionController& motionController;
                uint16_t rollValueHandle;
                uint16_t pitchValueHandle;
                uint16_t yawValueHandle;
                std::atomic_bool rollValuesNoficationEnabled {false};
                std::atomic_bool pitchValuesNoficationEnabled {false};
                std::atomic_bool yawValuesNoficationEnabled {false};
        };
    }
}