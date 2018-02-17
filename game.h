#ifndef GAME_H
#define GAME_H

#include <Arduino.h>
#include "globals.h"
#include "backdrop.h"
#include "elements.h"
#include "player.h"

void stateGamePlaying() {
 gameState = STATE_GAME_PLAYING;

 if (arduboy.pressed(RIGHT_BUTTON)) {
  if (player.state == PLAYER_SWINGING) {
    player_jump();
  } else {
    player_move(FACING_RIGHT);
  }
 }
 
 if (arduboy.pressed(LEFT_BUTTON)) {
    if (player.state == PLAYER_SWINGING) {
    player_jump();
  } else {
    player_move(FACING_LEFT);
  }
 }
 
 if (arduboy.pressed(DOWN_BUTTON)) {
  player_drop();
 }
 
 if (arduboy.pressed(UP_BUTTON)) {
  player_jump();
 }

 if (arduboy.pressed(B_BUTTON)) {
 }

 if (arduboy.pressed(A_BUTTON)) {
  player_throw_claw();
 }
 
 backdrop_draw();
 
 player_handle();
 level_element_handle();

};

void stateMenuPlay()
{
  backdrop.view_x = 0;
  backdrop.view_y = 0;

//  level_element_add(TYPE_XBOT,144,200,ENEMY_XBOT_LT,1,1);
  level_element_add(TYPE_XBOT,96,231,ENEMY_XBOT_LT,1,1);
//  level_element_add(TYPE_XBOT,240,208,ENEMY_XBOT_RT,1,1);

  player_init();
  stateGamePlaying();
 
};

void stageComplete() {
  gameState = STATE_STAGE_COMPLETE;
}

void stateStageComplete() {
    if (arduboy.justPressed(A_BUTTON | B_BUTTON)) stateMenuPlay();   
}


void stateGameOver() {
  if (arduboy.justPressed(A_BUTTON | B_BUTTON)) {
    gameState = STATE_MENU_MAIN;
  }
}



//-----------

#endif
