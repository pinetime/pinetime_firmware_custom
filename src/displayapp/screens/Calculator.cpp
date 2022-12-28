#include "displayapp/screens/Calculator.h"
#include <lvgl/lvgl.h>
#include "displayapp/DisplayApp.h"
#include "displayapp/InfiniTimeTheme.h"
#include "displayapp/screens/Symbols.h"

using namespace Pinetime::Applications::Screens;

Calculator::Calculator(Pinetime::Applications::DisplayApp* app,
                    System::SystemTask& systemTask)
: Screen(app),
systemTask{systemTask}
{
  systemTask.PushMessage(Pinetime::System::Messages::DisableSleeping);
}

Calculator::~Calculator() 
{
//   lv_task_del(taskRefresh);
//   lv_obj_clean(lv_scr_act());
  systemTask.PushMessage(Pinetime::System::Messages::EnableSleeping);
}