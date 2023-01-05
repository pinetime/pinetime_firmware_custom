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
  // lv_label_set_text_fmt(scoreText, "Start #FFFF00 %i#", score);
  //create head
  objSnake[0].head = lv_obj_create(lv_scr_act(), nullptr);
  lv_obj_set_style_local_bg_color(objSnake[0].head, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);
  lv_obj_set_size(objSnake[0].head, SIZE_X, SIZE_Y);
  objSnake[0].x = 50;
  objSnake[0].y = 50;
  lv_obj_set_pos(objSnake[0].head, objSnake[0].x, objSnake[0].y);
  //create bounding box
  _createBounder();
}

Snake::~Snake() 
{
  lv_task_del(taskRefresh);
  lv_obj_clean(lv_scr_act());
  systemTask.PushMessage(Pinetime::System::Messages::EnableSleeping);
}

void Snake::Refresh()
{
  if(objMove==right)
  {
    moveRight();
  }
  else if (objMove==left)
  {
    // moveLeft();
  }
  else if (objMove==up)
  {
    // moveUp();
  }
  else if (objMove==down)
  {
    // moveDown();
  }
  if(score == 5)
  {
    length = score;
    _snakeGrowUp();
  }
}

bool Snake::OnTouchEvent(Pinetime::Applications::TouchEvents event)
{
  switch (event)
  {
    case TouchEvents::SwipeRight:
      _updateGesture(TouchEvents::SwipeRight);
      _updateScore();
    return true;
    case TouchEvents::SwipeLeft:
      _updateGesture(TouchEvents::SwipeLeft);
      _updateScore();
    return true;
    case TouchEvents::SwipeUp:
      _updateGesture(TouchEvents::SwipeUp);
      _updateScore();
    return true;
    case TouchEvents::SwipeDown:
      _updateGesture(TouchEvents::SwipeDown);
      _updateScore();
    return true;
    default:
      return false;
  }
  return false;
}

void Snake::moveRight(void)
{
  for(uint8_t i=0; i < _maxNumberArray(); i++)
  {
    objSnake[i].x = objSnake[i].x + 2*STEP;
    objSnake[i].y = objSnake[0].y;
    lv_obj_set_pos(objSnake[i].head, 
                  objSnake[i].x, 
                  objSnake[i].y);
  }
}
void Snake::moveLeft(void)
{
  objSnake[0].x-=STEP;
  objSnake[0].y = objSnake[0].y;
}
void Snake::moveUp(void)
{
  objSnake[0].x = objSnake[0].x;
  objSnake[0].y-=STEP;
}
void Snake::moveDown(void)
{
  objSnake[0].x = objSnake[0].x;
  objSnake[0].y+=STEP;
}

uint8_t Snake::_updateScore(void)
{
  //test
  switch (objMove)
  {
    case right:
      score++;
      break;
    case left:
      score--;
      break;
    case up:
      score++;
      break;
    case down:
      score--;
      break;
  }
  //end
  lv_label_set_text_fmt(scoreText, 
                        "Your score: #FFFF00 %i#", 
                        score);
  return score;
}

uint8_t Snake::_updateGesture(TouchEvents event)
{

  switch (event)
  {
    case TouchEvents::SwipeRight:
      objMove = right; 
      break;
    case TouchEvents::SwipeLeft:
      objMove = left;
      break;
    case TouchEvents::SwipeUp:
      objMove = up;
      break;
    case TouchEvents::SwipeDown:
      objMove = down;
      break;
    default:
      objMove = none;
  }
  return objMove;
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

void Snake::_snakeGrowUp(void)
{
  uint8_t _arrSize = _maxNumberArray();
  if(length > _arrSize) //increase the length after eating the food
  {
    for(uint8_t i = _arrSize; i < length; i++)
    {
      objSnake[i].head = lv_obj_create(lv_scr_act(), nullptr);
      lv_obj_set_style_local_bg_color(objSnake[i].head, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);
      lv_obj_set_size(objSnake[i].head, SIZE_X, SIZE_Y);
      objSnake[i].x = objSnake[0].x-(i*4*STEP);
      objSnake[i].y = objSnake[i-1].y;
      lv_obj_set_pos(objSnake[i].head, objSnake[i].x, objSnake[i].y);
    }
  }
}

uint8_t Snake::_maxNumberArray(void)
{
  uint8_t _max=0;
  for(uint8_t i=0; i<100; i++)
  {
    if(objSnake[i].x == 0)
    {
      _max = i;
      break;
    }
  }
  return _max;
}