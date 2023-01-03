#pragma once

#include <cstdint>
#include <chrono>
#include "displayapp/screens/Screen.h"
#include "systemtask/SystemTask.h"
#include "lvgl/src/lv_core/lv_style.h"
#include "lvgl/src/lv_core/lv_obj.h"
#include "lvgl/src/lv_core/lv_style.h"
#include "lvgl/src/lv_widgets/lv_line.h"


namespace Pinetime
{
    namespace Applications
    {
        namespace Screens
        {
            class Snake : public Screen
            {
                public:
                Snake(Pinetime::Applications::DisplayApp* app,
                    System::SystemTask& systemTask);
                ~Snake() override;
                void OnNumEvent(lv_obj_t* obj, lv_event_t event);
                
                private:
                Pinetime::System::SystemTask& systemTask;
                lv_task_t* taskRefresh;
            };
        }
    }
}