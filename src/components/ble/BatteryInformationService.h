#pragma once
#define min // workaround: nimble's min/max macros conflict with libstdc++
#define max
#include <host/ble_gap.h>
#undef max
#undef min

namespace Pinetime {
  namespace System {
    class SystemTask;
  }
  namespace Controllers {
    class Battery;
    class BatteryInformationService {
    public:
      BatteryInformationService(Controllers::Battery& batteryController);
      void Init();

      int OnBatteryServiceRequested(uint16_t connectionHandle, uint16_t attributeHandle, ble_gatt_access_ctxt* context);
      void NotifyBatteryLevel(uint16_t connectionHandle, uint8_t level);
      void NotifyBatteryVoltage(uint16_t connectionHandle, uint8_t voltage);

    private:
      Controllers::Battery& batteryController;

      static constexpr uint16_t batteryInformationServiceId {0x180F};
      static constexpr uint16_t batteryLevelId {0x2A19};
      //uuid 47d80379-d624-4bb2-8e74-6f70a5e28655
      static constexpr uint8_t batteryVoltageId[16]=  {0x55,0x86,0xe2,0xa5,0x70,0x6f,0x74,0x8e,0xb2,0x4b,0x24,0xd6,0x19,0x03,0xd8,0x47};
      //uuid 0b5ced06-7293-4e59-9d4d-ed0c96ce9dc6
      static constexpr uint8_t extBatteryValueId[16]= {0xc6,0x9d,0xce,0x96,0x0c,0xed, 0x4d,0x9d,0x59,0x4e,0x93,0x72,0x06,0xed,0x5c,0x0b};

      static constexpr ble_uuid16_t batteryInformationServiceUuid {.u {.type = BLE_UUID_TYPE_16}, .value = batteryInformationServiceId};

      static constexpr ble_uuid16_t batteryLevelUuid 
      {.u {.type = BLE_UUID_TYPE_16}, 
      .value = batteryLevelId};
      
      static constexpr ble_uuid128_t batteryVoltageUuid 
      {.u {.type = BLE_UUID_TYPE_128}, 
      .value = {0x55,0x86,0xe2,0xa5,0x70,0x6f,0x74,0x8e,0xb2,0x4b,0x24,0xd6,0x19,0x03,0xd8,0x47}};

      static constexpr ble_uuid128_t extBatteryValueUuid 
      {.u {.type = BLE_UUID_TYPE_128}, 
      .value = {0xc6,0x9d,0xce,0x96,0x0c,0xed, 0x4d,0x9d,0x59,0x4e,0x93,0x72,0x06,0xed,0x5c,0x0b}};

      struct ble_gatt_chr_def characteristicDefinition[4];
      struct ble_gatt_svc_def serviceDefinition[2];

      uint16_t batteryLevelHandle;
      uint16_t batteryVoltageHandle;
      uint16_t extBatteryValueHandle;
    };
  }
}
