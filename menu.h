#ifndef MENU_H
#define MENU_H

#include "globals.h"
#include "font.h"


void drawTitleScreen()
{
  sprites.drawSelfMasked(20, 8, IMG_TITLE, 0);
}

void stateMenuIntro()
{
  globalCounter++;
  sprites.drawSelfMasked(34, 4, IMG_LOGO, 0);
  if (globalCounter > 120)
  {
    gameState = STATE_MENU_MAIN;
  }
}

void stateMenuMain()
{
  drawTitleScreen();
  print_progmem(16, 52, text_menu);
  print_progmem(((menuSelection-2)*25)+12, 52, text_pointer);
  if (arduboy.justPressed(RIGHT_BUTTON) && (menuSelection < 5)) menuSelection++;
  if (arduboy.justPressed(LEFT_BUTTON) && (menuSelection > 2)) menuSelection--;
  if (arduboy.justPressed(B_BUTTON)) gameState = menuSelection;
}

void stateMenuHelp()
{
  if (arduboy.justPressed(A_BUTTON | B_BUTTON)) gameState = STATE_MENU_MAIN;
}


void stateMenuInfo()
{
  drawTitleScreen();
  print_progmem(32, 52, text_info);
  if (arduboy.justPressed(A_BUTTON | B_BUTTON)) gameState = STATE_MENU_MAIN;
}

void stateMenuSoundfx()
{
  drawTitleScreen();
  print_progmem(29, 52, text_sound);
  print_progmem(59 + (arduboy.audio.enabled() * 23), 52, text_pointer);
  if (arduboy.justPressed(RIGHT_BUTTON)) arduboy.audio.on();
  if (arduboy.justPressed(LEFT_BUTTON)) arduboy.audio.off();
  if (arduboy.justPressed(A_BUTTON | B_BUTTON))
  {
    arduboy.audio.saveOnOff();
    gameState = STATE_MENU_MAIN;
  }
}

#endif
