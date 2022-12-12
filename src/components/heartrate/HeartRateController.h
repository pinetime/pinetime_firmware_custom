#pragma once

#include <cstdint>
#include <components/ble/HeartRateService.h>
#include <components/ble/BleAppCustomService.h>

namespace Pinetime {
  namespace Applications {
    class HeartRateTask;
  }
  namespace System {
    class SystemTask;
  }
  namespace Controllers {
    class HeartRateController {
    public:
      enum class States { Stopped, NotEnoughData, NoTouch, Running };

      HeartRateController() = default;
      void Start();
      void Stop();
      void Update(States newState, uint8_t heartRate);

      void SetHeartRateTask(Applications::HeartRateTask* task);
      States State() const {
        return state;
      }
      uint8_t HeartRate() const {
        return heartRate;
      }

      void SetService(Pinetime::Controllers::HeartRateService* service);
      void setCustomService(Pinetime::Controllers::BleAppCustomService *bleService);

    private:
      Applications::HeartRateTask* task = nullptr;
      States state = States::Stopped;
      uint8_t heartRate = 0;
      Pinetime::Controllers::HeartRateService* service = nullptr;
      Pinetime::Controllers::BleAppCustomService *bleService = nullptr;
    };
  }
}