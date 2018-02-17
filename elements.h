#ifndef ELEMENTS_H
#define ELEMENTS_H

#include "globals.h"
#include "backdrop.h"

#define MAX_ELEMENT_RECORDS 12

struct LevelElement
{
  unsigned char id;          //elment identifier
  unsigned char type;        //element type
  unsigned char x;           // y position
  unsigned char y;           // x position
  unsigned char state;       //determines the state of the the element
  unsigned char speed;       //deternimes how fast the object moves
  unsigned char step;        //used to maintain animation step
  unsigned char counter;     //used for timed events
  unsigned char speed_counter;
  unsigned char life;        //used to determine how much damage the element can take
};

LevelElement levelElements[MAX_ELEMENT_RECORDS];
char element_count = 0;

#define STATE_HIDDEN 0
#define STATE_DYING 99

#define TYPE_XBOT   5
#define TYPE_YBOT   6

//Enemy states
#define ENEMY_YBOT_LT 1
#define ENEMY_YBOT_RT 2

#define ENEMY_XBOT_LT 1
#define ENEMY_XBOT_RT 2

#define STEP_LENGTH  2
#define COUNTER_START 32

void level_element_add(char type, char x, char y, char state, char speed, char life)
{
      levelElements[element_count].id = element_count;
      levelElements[element_count].type = type;
      levelElements[element_count].x = x;
      levelElements[element_count].y = y;
      levelElements[element_count].step = 0;
      levelElements[element_count].state = state;
      levelElements[element_count].counter = COUNTER_START;
      levelElements[element_count].speed = speed;
      levelElements[element_count].speed_counter = 0;
      levelElements[element_count].life = life;
      element_count++;  
}

LevelElement level_element_get(char id) {
  return levelElements[id];
}

level_element_update(char id, LevelElement element) {
  levelElements[id] = element;
}

bool level_test_element (LevelElement element, char testX, char testY)
{
  char test_size = 8;
   if (element.type < 3) return false;
   if ((element.state == STATE_HIDDEN) ||(element.state == STATE_DYING)) return false;

   if ((element.y + test_size) < testY) return false;
   if (element.y > (testY + test_size)) return false;
   if ((element.x + test_size) < testX) return false;
   if (element.x > (testX + test_size)) return false;
   return true;
}


//*******************
LevelElement xbot_handle(LevelElement element)
{
  unsigned char adjust;
  if (element.state > STATE_HIDDEN)
  {
    element.step++;
    if (element.step > 1) element.step = 0;
    
    switch (element.state)
    { 
          case ENEMY_XBOT_LT:
            if (element.x > STEP_LENGTH) {
             if (backdrop_check_move_v(element.x - STEP_LENGTH, element.y+8, adjust) > 0 ) {
				    element.x -= STEP_LENGTH;
                    element.state = ENEMY_XBOT_LT;
             } else {
               element.state = ENEMY_XBOT_RT;
             }            
            }
            break;    

          case ENEMY_XBOT_RT:
            if (element.x < (255 - STEP_LENGTH)) {
             if (backdrop_check_move_v(element.x + STEP_LENGTH +8, element.y+8, adjust) >0 ) {
                    element.x += STEP_LENGTH;
                    element.state = ENEMY_XBOT_RT;
             } else {
                element.state = ENEMY_XBOT_LT;
             } 
            }
            break;
    }

    switch (element.state) {  
      case ENEMY_XBOT_LT:
      sprites.drawOverwrite(element.x - backdrop.view_x , element.y- backdrop.view_y, IMG_XBOT, element.step+2);
      break;

      case ENEMY_XBOT_RT:
      sprites.drawOverwrite(element.x - backdrop.view_x, element.y - backdrop.view_y, IMG_XBOT, element.step);
      break;
    }
  }
  return element;
}

//*******************
void level_element_handle()
{
  for (char i=0; i < element_count; i++)
  {     
    switch (levelElements[i].type) {
      case TYPE_XBOT:
      levelElements[i] = xbot_handle(levelElements[i]);
      break;
    }
    
  }
}

#endif
