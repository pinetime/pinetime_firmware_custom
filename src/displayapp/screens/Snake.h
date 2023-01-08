#pragma once

#include <cstdint>
#include <chrono>
#include "displayapp/screens/Screen.h"
#include "systemtask/SystemTask.h"
#include "lvgl/src/lv_core/lv_style.h"
#include "lvgl/src/lv_core/lv_obj.h"
#include "lvgl/src/lv_core/lv_style.h"
#include "lvgl/src/lv_widgets/lv_line.h"

#define STEP          4
#define SIZE_X        11
#define SIZE_Y        11
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
                void OnBtnEvent(lv_obj_t* obj, lv_event_t event);
                void moveRight(void);
                void moveLeft(void);
                void moveUp(void);
                void moveDown(void);
                void checkGameOver(void);
                void createFood(void);
                uint8_t length=1;
                typedef struct st_Snake
                {
                    lv_obj_t* head;
                    uint8_t x,y;
                };
                st_Snake objSnake[100] = {};
                typedef struct st_Food
                {
                    lv_obj_t* smallFood;
                    uint8_t f_x,f_y=0;
                    
                };
                st_Food objFood;
                private:
                Pinetime::System::SystemTask& systemTask;
                lv_task_t* taskRefresh;
                lv_obj_t* scoreText;
                lv_obj_t *replay_btn, *replay_label;
                lv_obj_t* replayText;
                
                //movement state 
                enum e_gesture {none, right, left, up, down};
                enum e_gesture objMove = none;
                enum e_gesture objLastMove = none;
                //operation state
                enum e_stateGame {state_none, start, stop, run};
                enum e_stateGame objStateGame= state_none;
                
                uint8_t score = 0;
                uint8_t _updateScore(void);
                uint8_t _updateGesture(TouchEvents event);
                void _createBounder(void);
                void _snakeGrowUp(void);
                uint8_t _maxSizeArray(void);
            };
        }
    }
}
