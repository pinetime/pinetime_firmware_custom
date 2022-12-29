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
            class Calculator : public Screen
            {
                public:
                Calculator(DisplayApp* app,
                        System::SystemTask& systemTask);
                ~Calculator() override;  
                //continously app updating
                void Refresh() override;
                void OnNumEvent(lv_obj_t* obj, lv_event_t event);
                
                
                private:
                Pinetime::System::SystemTask& systemTask;
                lv_task_t* taskRefresh;
                
                //keypad btn
                typedef struct
                {
                    lv_obj_t *num1,*num2,*num3,*num4,*num5,
                             *num6,*num7,*num8,*num9,*num0,
                             *numPlus,*numMinus,*numMultiply,
                             *numDivide, *numDel, *numEqual,
                             *label_1, *label_2, *label_3, *label_4, *label_5,
                             *label_6, *label_7, *label_8, *label_9, *label_0,
                             *label_Plus, *label_Minus, *label_Multiply, 
                             *label_Divide, *label_Del, *label_Equal;
                    //store ketypads' state
                uint8_t _stateBtn[4][4] {};
                }label;
                label ptr;
                void _createBounder(void);
            };
        }
    }
}

/**
 *  ____________________
 * |                    |
 * |____________________|
 * |  7 |  8 | 9  |  X  |
 * |  4 |  5 | 6  |  -  |
 * |  1 |  2 | 3  |  +  |
 * |  / |  0 | del|  =  |
 *  --------------------
 */