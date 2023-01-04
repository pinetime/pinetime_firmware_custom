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
                bool OnTouchEvent(TouchEvents event) override;
                
                private:
                Pinetime::System::SystemTask& systemTask;
                lv_task_t* taskRefresh;
                lv_obj_t* scoreText;
                uint8_t score = 0;
                uint8_t _updateScore(uint8_t _score);
                void _createBounder(void);
            };
        }
    }
}