#pragma once

#include <cstdint>
#include <chrono>
#include "displayapp/screens/Screen.h"
#include "systemtask/SystemTask.h"
#include "lvgl/src/lv_core/lv_style.h"
#include "lvgl/src/lv_core/lv_obj.h"

namespace Pinetime
{
    namespace Applications
    {
        namespace Screens
        {
            class Calculator : public Screen
            {
                public:
                Calculator(DisplayApp* app,
                        System::SystemTask& systemTask);
                ~Calculator() override;  
                private:
                Pinetime::System::SystemTask& systemTask;
            };
        }
    }
}