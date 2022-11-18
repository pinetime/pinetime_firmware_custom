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
    namespace Controllers
    {
        class BleAppCustomService
        {
            public:
                BleAppCustomService();
                void Init();
                int OnAppCustomServiceRequest(uint16_t connectionHandle, uint16_t attributeHandle, ble_gatt_access_ctxt* context);
            
            private:
                //uuid 98b54e7e-8803-4a34-819e-1012ac1cb497
                static constexpr uint16_t customId {0x2AAA};
                static constexpr const char* customName = "LOngnhan";
                static constexpr ble_uuid128_t bleAppCustomServiceUuid {.u {.type =BLE_UUID_TYPE_128}, 
                                                                        .value = {0x97,0xb4,0x1c,0xac,0x12,0x10,0x9e,0x81,0x34,0x4a,0x03,0x88,0x7e,0x4e,0xb5,0x98}};
                
                static constexpr ble_uuid16_t customUuid {.u {.type = BLE_UUID_TYPE_16}, 
                                                                        .value = customId};

                struct ble_gatt_chr_def characteristicDefinition[2];
                struct ble_gatt_svc_def serviceDefinition[2];
        };
    }
}