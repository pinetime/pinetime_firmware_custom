#pragma once

#include <cstdint>
#include <chrono>
#include "displayapp/screens/Screen.h"
#include "systemtask/SystemTask.h"
#include "lvgl/src/lv_core/lv_style.h"
#include "lvgl/src/lv_core/lv_obj.h"
#include <components/motion/MotionController.h>

namespace Pinetime
{
    namespace Controllers
    {
        class HeartRateController;
    }
    namespace Applications
    {
        namespace Screens
        {
            class MyApp : public Screen
            {
                public:
                    MyApp(DisplayApp* app,
                        Controllers::HeartRateController& HeartRateController, 
                        System::SystemTask& systemTask,
                        Controllers::MotionController& motionController);
                    ~MyApp() override;
                    //continously app updating
                    void Refresh() override;
                    void OnStartStopEvent(lv_event_t event);

                private:
                    Controllers::HeartRateController& heartRateController;
                    Controllers::MotionController& motionController;
                    Pinetime::System::SystemTask& systemTask;
                    
                    //HEART RATE LABELS
                    //heart rate value
                    lv_obj_t* label_hr;
                    //bpm text
                    lv_obj_t* label_bpm;
                    lv_obj_t* label_status;
                    lv_obj_t* btn_startStop;
                    lv_obj_t* label_startStop;
                    lv_task_t* taskRefresh;
                    
                    //MOTION LABELS
                    //X value
                    lv_obj_t* serX;
                    //Y value
                    lv_obj_t* serY;
                    //Z value
                    lv_obj_t* serZ;
                    //label for xyz values
                    lv_obj_t* label_x;
                    lv_obj_t* label_y;
                    lv_obj_t* label_z;

                    bool startMotonSensor=0;
                    void UpdateStartStopButton(bool isRunning);
            };
        }
    }
}