#pragma once

#include <cstdint>
#include <chrono>
#include "displayapp/screens/Screen.h"
#include "systemtask/SystemTask.h"
#include "lvgl/src/lv_core/lv_style.h"
#include "lvgl/src/lv_core/lv_obj.h"
#include "lvgl/src/lv_core/lv_style.h"
#include "lvgl/src/lv_widgets/lv_line.h"

#define STEP          2
#define SIZE_X        10
#define SIZE_Y        10
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
                void Refresh() override;
                bool OnTouchEvent(TouchEvents event) override;
                void moveRight(void);
                void moveLeft(void);
                void moveUp(void);
                void moveDown(void);
                uint8_t length=1;
                typedef struct st_Snake
                {
                    lv_obj_t* head;
                    uint8_t x,y;
                };
                st_Snake objSnake[100] = {};

                private:
                Pinetime::System::SystemTask& systemTask;
                lv_task_t* taskRefresh;
                lv_obj_t* scoreText;
                
                //movement state 
                enum e_gesture {none, right, left, up, down};
                enum e_gesture objMove = none;
                
                //system state
                enum e_system {state_none, run, stop};
                enum e_system objState;
                
                uint8_t score = 0;
                uint8_t countAddr = 0;
                uint8_t _updateScore(void);
                uint8_t _updateGesture(TouchEvents event);
                void _createBounder(void);
                void _snakeGrowUp(void);
                uint8_t _maxNumberArray(void);
            };
        }
    }
}
