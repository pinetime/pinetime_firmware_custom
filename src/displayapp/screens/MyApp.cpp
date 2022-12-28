#include "displayapp/screens/MyApp.h"
#include <lvgl/lvgl.h>
#include <components/heartrate/HeartRateController.h>
#include "displayapp/DisplayApp.h"
#include "displayapp/InfiniTimeTheme.h"
#include "displayapp/screens/Symbols.h"

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
  void btnBackEventHandler(lv_obj_t* obj, lv_event_t event)
  {
    auto* screen = static_cast<MyApp*>(obj->user_data);
    screen->OnBackEvent(event);
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
    lv_label_set_text_static(label_bpm, "* Heart rate: ");
    lv_obj_align(label_bpm, nullptr, LV_ALIGN_IN_TOP_LEFT, 10, 10);
    //create x label
    label_x = lv_label_create(lv_scr_act(), nullptr);
    lv_label_set_text_static(label_x, "* X value: ");
    lv_obj_align(label_x, label_bpm, LV_ALIGN_IN_LEFT_MID,0, 30);
    //create y label
    label_y = lv_label_create(lv_scr_act(), nullptr);
    lv_label_set_text_static(label_y, "* Y value: ");
    lv_obj_align(label_y, label_x, LV_ALIGN_IN_LEFT_MID,0, 30);
    //create z label
    label_z = lv_label_create(lv_scr_act(), nullptr);
    lv_label_set_text_static(label_z, "* Z value: ");
    lv_obj_align(label_z, label_y, LV_ALIGN_IN_LEFT_MID,0, 30);

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
  if(startMotionSensor==1)
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
  if(startChartValue==1)
  {
    //TODO replace X value by heart rate
    lv_chart_set_next(hrChart, hrSer, motionController.X());
    lv_label_set_text_fmt(label_chartLiveValue, "%03d", motionController.X());
  }
  else
  {

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
      startMotionSensor=1;
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
      startMotionSensor=0;
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
  //TODO find some way only allow to enter the event below ONCE from prevous page
  //better if we hide the chart button when only enter chart mode
  if (event == LV_EVENT_CLICKED)
  {
    startChartValue=1;
    lv_obj_set_hidden(label_hrChart, FALSE);
    //clear the whole objs
    lv_obj_set_hidden(label_x, TRUE); 
    lv_obj_set_hidden(label_y, TRUE); 
    lv_obj_set_hidden(label_z, TRUE);
    lv_obj_set_hidden(serX, TRUE);
    lv_obj_set_hidden(serY, TRUE); 
    lv_obj_set_hidden(serZ, TRUE);
    lv_obj_set_hidden(label_status, TRUE); 
    lv_obj_set_hidden(label_bpm, TRUE); 
    //hide 2 button whenever enter the chart view
    lv_obj_set_hidden(label_hr, TRUE);
    lv_obj_set_hidden(label_startStop, TRUE);
    lv_obj_set_hidden(btn_startStop, TRUE);
    lv_obj_set_hidden(btn_hrChart, TRUE);
    lv_obj_set_hidden(label_hrChart, TRUE);


    //display heart rate chart
    //display the chart content
    label_chartContent = lv_label_create(lv_scr_act(), nullptr);
    lv_label_set_text_static(label_chartContent, "Chart View");
    lv_obj_align(label_chartContent, nullptr, LV_ALIGN_IN_TOP_MID, 0, 0);

    //init chart display
    hrChart = lv_chart_create(lv_scr_act(), NULL);
    lv_obj_set_size(hrChart, 200, 200);
    lv_obj_align(hrChart, NULL, LV_ALIGN_IN_TOP_MID, 0, 0);
    lv_chart_set_type(hrChart, LV_CHART_TYPE_LINE); /*Show lines and points too*/
    lv_chart_set_range(hrChart, -1100, 1100);
    lv_chart_set_update_mode(hrChart, LV_CHART_UPDATE_MODE_SHIFT);
    lv_chart_set_point_count(hrChart, 10);
    hrSer = lv_chart_add_series(hrChart, LV_COLOR_BLUE);
    lv_chart_init_points(hrChart, hrSer, 0);
    lv_chart_refresh(hrChart); /*Required after direct set*/

    //display value while charting 
    label_chartLiveValue = lv_label_create(lv_scr_act(), nullptr);
    lv_label_set_text_static(label_chartLiveValue, "000");
    lv_obj_align(label_chartLiveValue, nullptr, LV_ALIGN_IN_BOTTOM_MID, 0, -25);
    

    //create turn back button
    btn_back = lv_btn_create(lv_scr_act(), nullptr);
    btn_back->user_data = this;
    lv_obj_set_height(btn_back, 45);
    lv_obj_set_width(btn_back, 60);
    lv_obj_set_event_cb(btn_back, btnBackEventHandler);
    lv_obj_align(btn_back, nullptr, LV_ALIGN_IN_BOTTOM_LEFT, 0, 0);
    label_back = lv_label_create(btn_back, nullptr);
    lv_label_set_text_static(label_back, Symbols::leftArrow);
  }
}

void MyApp::OnBackEvent(lv_event_t event)
{
  if (event == LV_EVENT_CLICKED)
  {
    startChartValue=0;
    //enable display previous page objects
    lv_obj_set_hidden(label_x, FALSE); 
    lv_obj_set_hidden(label_y, FALSE); 
    lv_obj_set_hidden(label_z, FALSE);
    lv_obj_set_hidden(serX, FALSE);
    lv_obj_set_hidden(serY, FALSE); 
    lv_obj_set_hidden(serZ, FALSE);
    lv_obj_set_hidden(label_status, FALSE); 
    lv_obj_set_hidden(label_bpm, FALSE); 
    lv_obj_set_hidden(label_hr, FALSE);
    lv_obj_set_hidden(label_startStop, FALSE);
    lv_obj_set_hidden(btn_startStop, FALSE);
    lv_obj_set_hidden(label_hrChart, FALSE);
    lv_obj_set_hidden(btn_hrChart, FALSE);
    
    //hide chart objects
    lv_obj_set_hidden(label_chartContent, TRUE);
    lv_chart_hide_series(hrChart, hrSer, TRUE);
    lv_obj_set_hidden(btn_back, TRUE);
    lv_obj_set_hidden(label_back, TRUE);
    lv_obj_set_hidden(label_chartLiveValue, TRUE);
  }
}

void MyApp::UpdateStartStopButton(bool isRunning) 
{
  if (isRunning)
    lv_label_set_text_static(label_startStop, "Stop");
  else
    lv_label_set_text_static(label_startStop, "Start");
}
