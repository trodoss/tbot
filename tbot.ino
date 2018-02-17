/*
TBOT

2017/2018 - Trodoss
	
*/


#include "globals.h"
#include "menu.h"
#include "game.h"

typedef void (*FunctionPointer) ();

const FunctionPointer PROGMEM mainGameLoop[] = {
  stateMenuIntro,
  stateMenuMain,
  stateMenuHelp,
  stateMenuPlay,
  stateMenuInfo,
  stateMenuSoundfx,
  stateGamePlaying,
  stateStageComplete,
  stateGameOver
};


void setup() {
  arduboy.begin();
  arduboy.setFrameRate(30);                                 // set the frame rate of the game at 30 fps
  arduboy.initRandomSeed();                                 // This sets the random to more random, remove this if no random is needed !
}


void loop() {
  if (!(arduboy.nextFrame())) return;
  arduboy.pollButtons();
  arduboy.clear();
((FunctionPointer) pgm_read_word (&mainGameLoop[gameState]))();
  arduboy.display();
  if (arduboy.everyXFrames(3)) blinking = !blinking;
}

