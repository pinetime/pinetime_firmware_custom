#include "displayapp/screens/MyApp.h"
#include <lvgl/lvgl.h>
#include <components/heartrate/HeartRateController.h>
#include "displayapp/DisplayApp.h"
#include "displayapp/InfiniTimeTheme.h"

using namespace Pinetime::Applications::Screens;

namespace 
{
  const char* ToString(Pinetime::Controllers::HeartRateController::States s) 
  {
    switch (s) 
    {
      case Pinetime::Controllers::HeartRateController::States::NotEnoughData:
        return "Not enough data,\nplease wait...";
      case Pinetime::Controllers::HeartRateController::States::NoTouch:
        return "No touch detected";
      case Pinetime::Controllers::HeartRateController::States::Running:
        return "Measuring...";
      case Pinetime::Controllers::HeartRateController::States::Stopped:
        return "Stopped";
    }
    return "";
  }

  void btnStartStopEventHandler(lv_obj_t* obj, lv_event_t event) 
  {
    auto* screen = static_cast<MyApp*>(obj->user_data);
    screen->OnStartStopEvent(event);
  }
  void btnHrChartEventHandler(lv_obj_t* obj, lv_event_t event)
  {
    auto* screen = static_cast<MyApp*>(obj->user_data);
    screen->OnHrChartEvent(event);
  }
}

MyApp::MyApp(Pinetime::Applications::DisplayApp* app,
                     Controllers::HeartRateController& heartRateController,
                     System::SystemTask& systemTask,
                     Controllers::MotionController& motionController)
  : Screen(app), 
  heartRateController {heartRateController}, 
  systemTask {systemTask},
  motionController {motionController} 
  {
    bool isHrRunning = heartRateController.State() != Controllers::HeartRateController::States::Stopped;
    
    //create bpm label
    label_bpm = lv_label_create(lv_scr_act(), nullptr);
    lv_label_set_text_static(label_bpm, "Heart rate: ");
    lv_obj_align(label_bpm, nullptr, LV_ALIGN_IN_TOP_LEFT, 0, 0);
    //create x label
    label_x = lv_label_create(lv_scr_act(), nullptr);
    lv_label_set_text_static(label_x, "x value: ");
    lv_obj_align(label_x, label_bpm, LV_ALIGN_IN_LEFT_MID,0, 25);
    //create y label
    label_y = lv_label_create(lv_scr_act(), nullptr);
    lv_label_set_text_static(label_y, "y value: ");
    lv_obj_align(label_y, label_x, LV_ALIGN_IN_LEFT_MID,0, 25);
    //create z label
    label_z = lv_label_create(lv_scr_act(), nullptr);
    lv_label_set_text_static(label_z, "z value: ");
    lv_obj_align(label_z, label_y, LV_ALIGN_IN_LEFT_MID,0, 25);

    //create heart rate label
    label_hr = lv_label_create(lv_scr_act(), nullptr);
    lv_label_set_text_static(label_hr, "000"); 
    lv_obj_align(label_hr, label_bpm, LV_ALIGN_IN_RIGHT_MID, 25, 0);
    //create serX label
    serX = lv_label_create(lv_scr_act(), nullptr);
    lv_label_set_text_static(serX, "000"); 
    lv_obj_align(serX, label_x, LV_ALIGN_IN_RIGHT_MID,25, 0);
    //create serY label
    serY = lv_label_create(lv_scr_act(), nullptr);
    lv_label_set_text_static(serY, "000"); 
    lv_obj_align(serY, label_y, LV_ALIGN_IN_RIGHT_MID,25, 0);
    //create serZ label
    serZ = lv_label_create(lv_scr_act(), nullptr);
    lv_label_set_text_static(serZ, "000"); 
    lv_obj_align(serZ, label_z, LV_ALIGN_IN_RIGHT_MID,25, 0);

    label_status = lv_label_create(lv_scr_act(), nullptr);
    lv_obj_set_style_local_text_color(label_status, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_GRAY);
    lv_label_set_text_static(label_status, ToString(Pinetime::Controllers::HeartRateController::States::NotEnoughData));

    lv_obj_align(label_status, label_hr, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);

    //create start-stop button
    btn_startStop = lv_btn_create(lv_scr_act(), nullptr);
    btn_startStop->user_data = this;
    //set height and width for the start-stop button
    lv_obj_set_height(btn_startStop, 45);
    lv_obj_set_width(btn_startStop, 70);
    lv_obj_set_event_cb(btn_startStop, btnStartStopEventHandler);
    lv_obj_align(btn_startStop, nullptr, LV_ALIGN_IN_BOTTOM_LEFT, 0, 0);
    label_startStop = lv_label_create(btn_startStop, nullptr);

    //create heart rate chart button
    btn_hrChart = lv_btn_create(lv_scr_act(), nullptr);
    btn_hrChart->user_data = this;
    //set height and width for the start-stop button
    lv_obj_set_height(btn_hrChart, 45);
    lv_obj_set_width(btn_hrChart, 70);
    lv_obj_set_event_cb(btn_hrChart, btnHrChartEventHandler);
    lv_obj_align(btn_hrChart, nullptr, LV_ALIGN_IN_BOTTOM_RIGHT, 0, 0);
    label_hrChart = lv_label_create(btn_hrChart, nullptr);
    lv_label_set_text_static(label_hrChart, "Chart");


    UpdateStartStopButton(isHrRunning);
    if (isHrRunning)
      systemTask.PushMessage(Pinetime::System::Messages::DisableSleeping);

    taskRefresh = lv_task_create(RefreshTaskCallback, 100, LV_TASK_PRIO_MID, this);
  }

MyApp::~MyApp() 
{
  lv_task_del(taskRefresh);
  lv_obj_clean(lv_scr_act());
  systemTask.PushMessage(Pinetime::System::Messages::EnableSleeping);
}

void MyApp::Refresh() 
{
  auto state = heartRateController.State();
  switch (state) 
  {
    case Controllers::HeartRateController::States::NoTouch:
    case Controllers::HeartRateController::States::NotEnoughData:
      lv_label_set_text_static(label_hr, "000");
      break;
    default:
      lv_label_set_text_fmt(label_hr, "%03d", heartRateController.HeartRate());
  }

  lv_label_set_text_static(label_status, ToString(state));
  lv_obj_align(label_status, nullptr, LV_ALIGN_IN_BOTTOM_MID, 0, -70);

  //handle motion values
  if(startMotonSensor==1)
  {
    lv_label_set_text_fmt(serX, "%03d", motionController.X());
    lv_label_set_text_fmt(serY, "%03d", motionController.Y());
    lv_label_set_text_fmt(serZ, "%03d", motionController.Z());
  }
  else
  {
    lv_label_set_text_static(serX, "000");
    lv_label_set_text_static(serY, "000");
    lv_label_set_text_static(serZ, "000");
  }
  
}

void MyApp::OnStartStopEvent(lv_event_t event) 
{
  if (event == LV_EVENT_CLICKED) 
  {
    if (heartRateController.State() == Controllers::HeartRateController::States::Stopped) 
    {
      heartRateController.Start();
      //enable read write the motion sensor
      startMotonSensor=1;
      UpdateStartStopButton(heartRateController.State() != Controllers::HeartRateController::States::Stopped);
      systemTask.PushMessage(Pinetime::System::Messages::DisableSleeping);
      lv_obj_set_style_local_text_color(label_hr, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, Colors::highlight);
      lv_obj_set_style_local_text_color(serX, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, Colors::highlight);
      lv_obj_set_style_local_text_color(serY, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, Colors::highlight);
      lv_obj_set_style_local_text_color(serZ, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, Colors::highlight);
    } 
    else 
    {
      heartRateController.Stop();
      //enable read write the motion sensor
      startMotonSensor=0;
      UpdateStartStopButton(heartRateController.State() != Controllers::HeartRateController::States::Stopped);
      systemTask.PushMessage(Pinetime::System::Messages::EnableSleeping);
      lv_obj_set_style_local_text_color(label_hr, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, Colors::lightGray);
      lv_obj_set_style_local_text_color(serX, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, Colors::lightGray);
      lv_obj_set_style_local_text_color(serY, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, Colors::lightGray);
      lv_obj_set_style_local_text_color(serZ, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, Colors::lightGray);
    }
  }
}

void MyApp::OnHrChartEvent(lv_event_t event)
{
  if (event == LV_EVENT_CLICKED)
  {

  }
}

void MyApp::UpdateStartStopButton(bool isRunning) 
{
  if (isRunning)
    lv_label_set_text_static(label_startStop, "Stop");
  else
    lv_label_set_text_static(label_startStop, "Start");
}
