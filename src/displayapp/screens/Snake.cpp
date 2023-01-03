#include "displayapp/screens/Snake.h"
#include <lvgl/lvgl.h>
#include "displayapp/DisplayApp.h"
#include "displayapp/InfiniTimeTheme.h"
#include "displayapp/screens/Symbols.h"

using namespace Pinetime::Applications::Screens;
namespace
{
  void btnNumEventHandler(lv_obj_t* obj, lv_event_t event)
  {
    auto* screen = static_cast<Snake*>(obj->user_data);
    screen->OnNumEvent(obj, event);
  }
}

Snake::Snake(Pinetime::Applications::DisplayApp* app,
                    System::SystemTask& systemTask)
: Screen(app),
systemTask{systemTask}
{
  systemTask.PushMessage(Pinetime::System::Messages::DisableSleeping);
  taskRefresh = lv_task_create(RefreshTaskCallback, 100, LV_TASK_PRIO_MID, this);
}

Snake::~Snake() 
{
  lv_task_del(taskRefresh);
  lv_obj_clean(lv_scr_act());
  systemTask.PushMessage(Pinetime::System::Messages::EnableSleeping);
}

void Snake::OnNumEvent(lv_obj_t* obj, lv_event_t event)
{

}


