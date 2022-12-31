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
                void updateDisplayX(void);
                void updateDisplayY(void);
                
                private:
                Pinetime::System::SystemTask& systemTask;
                lv_task_t* taskRefresh;
                lv_obj_t *label_x;
                lv_obj_t *label_y;
                lv_obj_t *label_sum;
                int32_t varX=0;
                int32_t varY=0;
                float_t varSum=0;
                int32_t varSumInt=0;
                lv_obj_t *label_mathSymbol;
                
                //keypad btn
                typedef struct
                {
                    enum e_numVal
                    {
                        val_num0=0, val_num1, val_num2, 
                        val_num3, val_num4, val_num5, 
                        val_num6, val_num7, val_num8, val_num9
                    };

                    enum e_mathSymbol
                    {
                        math_none=0, 
                        math_add, 
                        math_minus, 
                        math_multiply, 
                        math_divide
                    };
                    enum e_mathSymbol currentMathSymbol;

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
                bool _isTypeY=0;
                bool _isGetSumValue=0;
                bool _isFloatingPoint=0;
                int8_t _minusPrefix=1;
                void _createBounder(void);
            };
        }
    }
}

/**
 * 
 *  MY CALCULATOR
 *  ____________________
 * |                    |
 * |____________________|
 * |  7 |  8 | 9  |  +  |
 * |  4 |  5 | 6  |  -  |
 * |  1 |  2 | 3  |  x  |
 * |  = |  0 | del|  /  |
 *  --------------------
 */