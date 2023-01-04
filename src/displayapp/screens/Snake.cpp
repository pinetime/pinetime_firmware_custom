#include "displayapp/screens/Snake.h"
#include <lvgl/lvgl.h>
#include "displayapp/DisplayApp.h"
#include "displayapp/InfiniTimeTheme.h"
#include "displayapp/screens/Symbols.h"

using namespace Pinetime::Applications::Screens;

Snake::Snake(Pinetime::Applications::DisplayApp* app,
                    System::SystemTask& systemTask)
: Screen(app),
systemTask{systemTask}
{
  systemTask.PushMessage(Pinetime::System::Messages::DisableSleeping);
  taskRefresh = lv_task_create(RefreshTaskCallback, 100, LV_TASK_PRIO_MID, this);
  //create score
  scoreText = lv_label_create(lv_scr_act(), nullptr);
  lv_obj_set_width(scoreText, LV_HOR_RES);
  lv_label_set_align(scoreText, LV_ALIGN_IN_LEFT_MID);
  lv_obj_align(scoreText, nullptr, LV_ALIGN_IN_TOP_LEFT, 0, 0);
  lv_label_set_recolor(scoreText, true);
  lv_label_set_text_fmt(scoreText, "Score #FFFF00 %i#", score);
  //create bounding box
  _createBounder();
}

Snake::~Snake() 
{
  lv_task_del(taskRefresh);
  lv_obj_clean(lv_scr_act());
  systemTask.PushMessage(Pinetime::System::Messages::EnableSleeping);
}

bool Snake::OnTouchEvent(Pinetime::Applications::TouchEvents event)
{
  switch (event)
  {
    case TouchEvents::SwipeRight:
      score++;
      _updateScore(score);
    return true;
    case TouchEvents::SwipeLeft:
      score--;
      _updateScore(score);
    return true;
    case TouchEvents::SwipeUp:
      score++;
      _updateScore(score);
    return true;
    case TouchEvents::SwipeDown:
      score--;
      _updateScore(score);
    return true;
    default:
      return false;
  }
  return false;
}

uint8_t Snake::_updateScore(uint8_t _score)
{
  lv_label_set_text_fmt(scoreText, "Your score: #FFFF00 %i#", _score);
  return _score;
}

void Snake::_createBounder(void)
{
    /*Create an array for the points of the line*/
    /**
     * (x1[0],y1[1])-----------------(x2[2],y1[1])
     *    |                                 |
     * (x1[0],y2[3])-----------------(x2[2],y2[3])
     */
    uint8_t _arr[4]={0,24,240,220};
    static lv_point_t line_points[] = { {_arr[0], _arr[1]}, {_arr[2], _arr[1]}, 
                  {_arr[2], _arr[3]}, {_arr[0], _arr[3]}, {_arr[0], _arr[1]}};

    /*Create style*/
    static lv_style_t style_line;
    lv_style_init(&style_line);
    lv_style_set_line_width(&style_line, LV_STATE_DEFAULT, 5);
    lv_style_set_line_color(&style_line, LV_STATE_DEFAULT, LV_COLOR_GREEN);
    lv_style_set_line_rounded(&style_line, LV_STATE_DEFAULT, true);

    /*Create a line and apply the new style*/
    lv_obj_t * line1;
    line1 = lv_line_create(lv_scr_act(), NULL);
    lv_line_set_points(line1, line_points, 5);     /*Set the points*/
    lv_obj_add_style(line1, LV_STATE_DEFAULT, &style_line);
}


