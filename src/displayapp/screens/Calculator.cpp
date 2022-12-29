#include "displayapp/screens/Calculator.h"
#include <lvgl/lvgl.h>
#include "displayapp/DisplayApp.h"
#include "displayapp/InfiniTimeTheme.h"
#include "displayapp/screens/Symbols.h"

using namespace Pinetime::Applications::Screens;
namespace
{
  void btnNumEventHandler(lv_obj_t* obj, lv_event_t event)
  {
    auto* screen = static_cast<Calculator*>(obj->user_data);
    screen->OnNumEvent(obj, event);
  }
}

Calculator::Calculator(Pinetime::Applications::DisplayApp* app,
                    System::SystemTask& systemTask)
: Screen(app),
systemTask{systemTask}
{
  //create all keypads' calculator
  systemTask.PushMessage(Pinetime::System::Messages::DisableSleeping);
  taskRefresh = lv_task_create(RefreshTaskCallback, 100, LV_TASK_PRIO_MID, this);
  //create the bounder
  _createBounder();

  //TODO bring all num and label into a array to created once
  //TODO separate lv_obj_align from obj creation

  //btn 7
  ptr.num7 = lv_btn_create(lv_scr_act(), nullptr);
  ptr.num7->user_data = this;
  lv_obj_set_height(ptr.num7, 45);
  lv_obj_set_width(ptr.num7, 56);
  lv_obj_set_event_cb(ptr.num7, btnNumEventHandler);
  lv_obj_align(ptr.num7, nullptr, LV_ALIGN_IN_TOP_LEFT, 0, 50);
  ptr.label_7 = lv_label_create(ptr.num7, nullptr);
  lv_label_set_text_static(ptr.label_7, "7");
  //btn 8
  ptr.num8 = lv_btn_create(lv_scr_act(), nullptr);
  ptr.num8->user_data = this;
  lv_obj_set_height(ptr.num8, 45);
  lv_obj_set_width(ptr.num8, 56);
  lv_obj_set_event_cb(ptr.num8, btnNumEventHandler);
  lv_obj_align(ptr.num8, ptr.num7, LV_ALIGN_IN_TOP_LEFT, 60, 0);
  ptr.label_8 = lv_label_create(ptr.num8, nullptr);
  lv_label_set_text_static(ptr.label_8, "8");
  //btn 9
  ptr.num9 = lv_btn_create(lv_scr_act(), nullptr);
  ptr.num9->user_data = this;
  lv_obj_set_height(ptr.num9, 45);
  lv_obj_set_width(ptr.num9, 56);
  lv_obj_set_event_cb(ptr.num9, btnNumEventHandler);
  lv_obj_align(ptr.num9, ptr.num8, LV_ALIGN_IN_TOP_LEFT, 60, 0);
  ptr.label_9 = lv_label_create(ptr.num9, nullptr);
  lv_label_set_text_static(ptr.label_9, "9");
  //btn plus
  ptr.numPlus = lv_btn_create(lv_scr_act(), nullptr);
  ptr.numPlus->user_data = this;
  lv_obj_set_height(ptr.numPlus, 45);
  lv_obj_set_width(ptr.numPlus, 56);
  lv_obj_set_event_cb(ptr.numPlus, btnNumEventHandler);
  lv_obj_align(ptr.numPlus, ptr.num9, LV_ALIGN_IN_TOP_LEFT, 60, 0);
  ptr.label_Plus = lv_label_create(ptr.numPlus, nullptr);
  lv_label_set_text_static(ptr.label_Plus, Symbols::plus);
  // btn 4
  ptr.num4 = lv_btn_create(lv_scr_act(), nullptr);
  ptr.num4->user_data = this;
  lv_obj_set_height(ptr.num4, 45);
  lv_obj_set_width(ptr.num4, 56);
  lv_obj_set_event_cb(ptr.num4, btnNumEventHandler);
  lv_obj_align(ptr.num4, ptr.num7, LV_ALIGN_IN_TOP_LEFT, 0, 50);
  ptr.label_4 = lv_label_create(ptr.num4, nullptr);
  lv_label_set_text_static(ptr.label_4, "4");
  //btn 5
  ptr.num5 = lv_btn_create(lv_scr_act(), nullptr);
  ptr.num5->user_data = this;
  lv_obj_set_height(ptr.num5, 45);
  lv_obj_set_width(ptr.num5, 56);
  lv_obj_set_event_cb(ptr.num5, btnNumEventHandler);
  lv_obj_align(ptr.num5, ptr.num4, LV_ALIGN_IN_TOP_LEFT, 60, 0);
  ptr.label_5 = lv_label_create(ptr.num5, nullptr);
  lv_label_set_text_static(ptr.label_5, "5");
  //btn 6
  ptr.num6 = lv_btn_create(lv_scr_act(), nullptr);
  ptr.num6->user_data = this;
  lv_obj_set_height(ptr.num6, 45);
  lv_obj_set_width(ptr.num6, 56);
  lv_obj_set_event_cb(ptr.num6, btnNumEventHandler);
  lv_obj_align(ptr.num6, ptr.num5, LV_ALIGN_IN_TOP_LEFT, 60, 0);
  ptr.label_6 = lv_label_create(ptr.num6, nullptr);
  lv_label_set_text_static(ptr.label_6, "6");
  //btn minus
  ptr.numMinus = lv_btn_create(lv_scr_act(), nullptr);
  ptr.numMinus->user_data = this;
  lv_obj_set_height(ptr.numMinus, 45);
  lv_obj_set_width(ptr.numMinus, 56);
  lv_obj_set_event_cb(ptr.numMinus, btnNumEventHandler);
  lv_obj_align(ptr.numMinus, ptr.num6, LV_ALIGN_IN_TOP_LEFT, 60, 0);
  ptr.label_Minus = lv_label_create(ptr.numMinus, nullptr);
  lv_label_set_text_static(ptr.label_Minus, Symbols::minus);
  // btn 1
  ptr.num1 = lv_btn_create(lv_scr_act(), nullptr);
  ptr.num1->user_data = this;
  lv_obj_set_height(ptr.num1, 45);
  lv_obj_set_width(ptr.num1, 56);
  lv_obj_set_event_cb(ptr.num1, btnNumEventHandler);
  lv_obj_align(ptr.num1, ptr.num4, LV_ALIGN_IN_TOP_LEFT, 0, 50);
  ptr.label_1 = lv_label_create(ptr.num1, nullptr);
  lv_label_set_text_static(ptr.label_1, "1");
  //btn 2
  ptr.num2 = lv_btn_create(lv_scr_act(), nullptr);
  ptr.num2->user_data = this;
  lv_obj_set_height(ptr.num2, 45);
  lv_obj_set_width(ptr.num2, 56);
  lv_obj_set_event_cb(ptr.num2, btnNumEventHandler);
  lv_obj_align(ptr.num2, ptr.num1, LV_ALIGN_IN_TOP_LEFT, 60, 0);
  ptr.label_2 = lv_label_create(ptr.num2, nullptr);
  lv_label_set_text_static(ptr.label_2, "2");
  //btn 3
  ptr.num3 = lv_btn_create(lv_scr_act(), nullptr);
  ptr.num3->user_data = this;
  lv_obj_set_height(ptr.num3, 45);
  lv_obj_set_width(ptr.num3, 56);
  lv_obj_set_event_cb(ptr.num3, btnNumEventHandler);
  lv_obj_align(ptr.num3, ptr.num2, LV_ALIGN_IN_TOP_LEFT, 60, 0);
  ptr.label_3 = lv_label_create(ptr.num3, nullptr);
  lv_label_set_text_static(ptr.label_3, "3");
  //btn multiply
  ptr.numMultiply = lv_btn_create(lv_scr_act(), nullptr);
  ptr.numMultiply->user_data = this;
  lv_obj_set_height(ptr.numMultiply, 45);
  lv_obj_set_width(ptr.numMultiply, 56);
  lv_obj_set_event_cb(ptr.numMultiply, btnNumEventHandler);
  lv_obj_align(ptr.numMultiply, ptr.num3, LV_ALIGN_IN_TOP_LEFT, 60, 0);
  ptr.label_Multiply = lv_label_create(ptr.numMultiply, nullptr);
  lv_label_set_text_static(ptr.label_Multiply, Symbols::multiply);
  //btn equal
  ptr.numEqual = lv_btn_create(lv_scr_act(), nullptr);
  ptr.numEqual->user_data = this;
  lv_obj_set_height(ptr.numEqual, 45);
  lv_obj_set_width(ptr.numEqual, 56);
  lv_obj_set_event_cb(ptr.numEqual, btnNumEventHandler);
  lv_obj_align(ptr.numEqual, ptr.num1, LV_ALIGN_IN_TOP_LEFT, 0, 50);
  ptr.label_Equal = lv_label_create(ptr.numEqual, nullptr);
  lv_label_set_text_static(ptr.label_Equal, Symbols::equal);
  //btn 0
  ptr.num0 = lv_btn_create(lv_scr_act(), nullptr);
  ptr.num0->user_data = this;
  lv_obj_set_height(ptr.num0, 45);
  lv_obj_set_width(ptr.num0, 56);
  lv_obj_set_event_cb(ptr.num0, btnNumEventHandler);
  lv_obj_align(ptr.num0, ptr.numEqual, LV_ALIGN_IN_TOP_LEFT, 60, 0);
  ptr.label_0 = lv_label_create(ptr.num0, nullptr);
  lv_label_set_text_static(ptr.label_0, "0");
  //btn del
  ptr.numDel = lv_btn_create(lv_scr_act(), nullptr);
  ptr.numDel->user_data = this;
  lv_obj_set_height(ptr.numDel, 45);
  lv_obj_set_width(ptr.numDel, 56);
  lv_obj_set_event_cb(ptr.numDel, btnNumEventHandler);
  lv_obj_align(ptr.numDel, ptr.num0, LV_ALIGN_IN_TOP_LEFT, 60, 0);
  ptr.label_Del = lv_label_create(ptr.numDel, nullptr);
  lv_label_set_text_static(ptr.label_Del,Symbols::backpsace);
  //btn divide
  ptr.numDivide = lv_btn_create(lv_scr_act(), nullptr);
  ptr.numDivide->user_data = this;
  lv_obj_set_height(ptr.numDivide, 45);
  lv_obj_set_width(ptr.numDivide, 56);
  lv_obj_set_event_cb(ptr.numDivide, btnNumEventHandler);
  lv_obj_align(ptr.numDivide, ptr.numDel, LV_ALIGN_IN_TOP_LEFT, 60, 0);
  ptr.label_Divide = lv_label_create(ptr.numDivide, nullptr);
  lv_label_set_text_static(ptr.label_Divide,Symbols::divide);
}

Calculator::~Calculator() 
{
  lv_task_del(taskRefresh);
  lv_obj_clean(lv_scr_act());
  systemTask.PushMessage(Pinetime::System::Messages::EnableSleeping);
}

void Calculator::Refresh()
{
  //class values continously updating 
}

void Calculator::OnNumEvent(lv_obj_t* obj, lv_event_t event)
{
  if (event == LV_EVENT_CLICKED)
  {
    if(obj == ptr.num7)
    {
      //test
      // lv_obj_set_style_local_text_color(ptr.label_3, 
      //                         LV_LABEL_PART_MAIN, 
      //                         LV_STATE_DEFAULT, 
      //                         LV_COLOR_RED);
    }
    if(obj == ptr.num8)
    {
      // lv_obj_set_style_local_text_color(ptr.label_3, 
      //                         LV_LABEL_PART_MAIN, 
      //                         LV_STATE_DEFAULT, 
      //                         LV_COLOR_WHITE);
    }
    if(obj == ptr.num9)
    {
     
    }
    if(obj == ptr.numPlus)
    {
      return;
    }
    if(obj == ptr.num4)
    {
      return;
    }
    if(obj == ptr.num5)
    {
      return;
    }
    if(obj == ptr.num6)
    {
      return;
    }
    if(obj == ptr.numMinus)
    {
      return;
    }
    if(obj == ptr.num1)
    {

    }
    if(obj == ptr.num2)
    {

    }
    if(obj == ptr.num3)
    {

    }
    if(obj == ptr.numMultiply)
    {

    }
    if(obj == ptr.numEqual)
    {

    }
    if(obj == ptr.num0)
    {

    }
    if(obj == ptr.numDel)
    {

    }
    if(obj == ptr.numDivide)
    {

    }
  }
}

void Calculator::_createBounder(void)
{
    /*Create an array for the points of the line*/
    //Bounder's dimension is 240x42
    static lv_point_t line_points[] = { {0, 0}, {240, 0}, {240, 42}, {0, 42}, {0, 0} };

    /*Create style*/
    static lv_style_t style_line;
    lv_style_init(&style_line);
    lv_style_set_line_width(&style_line, LV_STATE_DEFAULT, 5);
    lv_style_set_line_color(&style_line, LV_STATE_DEFAULT, LV_COLOR_YELLOW);
    lv_style_set_line_rounded(&style_line, LV_STATE_DEFAULT, true);

    /*Create a line and apply the new style*/
    lv_obj_t * line1;
    line1 = lv_line_create(lv_scr_act(), NULL);
    lv_line_set_points(line1, line_points, 5);     /*Set the points*/
    lv_obj_add_style(line1, LV_STATE_DEFAULT, &style_line);
}