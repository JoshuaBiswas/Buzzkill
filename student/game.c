#include "game.h"

#include <stdio.h>
#include <stdlib.h>

#include "gba.h"
#include "images/startimage.h"
#include "images/winimage.h"
#include "images/loseimage.h"
#include "play.h"

typedef enum {
  START,
  PLAY,
  WIN,
  LOSE,
} GBAState;

int main(void) {
  //mode 3 and display 2 settings
  REG_DISPCNT = MODE3 | BG2_ENABLE;

  // Save current and previous state of button input.
  u32 previousButtons = BUTTONS;
  u32 currentButtons = BUTTONS;

  // Load initial game state
  GBAState state = START;

  while (1) {
    currentButtons = BUTTONS;  // Load the current state of the buttons

    //this int stores the result of play() when it is called during the PLAY state
    int gameResult;
    
    switch (state) {
      case START:
        //this draws the title screen image and waits for user to hit START to play
        waitForVBlank();
        drawFullScreenImageDMA(startimage);
        drawCenteredString(140, 120, 0, 0, "Press START to play...", WHITE);
        if (((~currentButtons & BUTTON_START) == 0) && ((~previousButtons & BUTTON_START) != 0)) {
          state = PLAY;
        }
        break;
      case PLAY:
        //this calls the play() function, stores its value to an int, and uses this int to determine next state (0 -> title screen, 1 -> win, 2 -> loss).
        gameResult = play();
        if (gameResult == 0) {
          state = START;
        } else if (gameResult == 1) {
          state = WIN;
        } else {
          state = LOSE;
        }
        break;
      case WIN:
        //this draws the win screen image and waits for user to hit SELECT to return to title screen
        waitForVBlank();
        drawFullScreenImageDMA(winimage);
        drawCenteredString(140, 120, 0, 0, "Press SELECT to return to start menu...", WHITE);
        if (((~currentButtons & BUTTON_SELECT) == 0) && ((~previousButtons & BUTTON_SELECT) != 0)) {
          state = START;
        }
        break;
      case LOSE:
        //this draws the lose screen image and waits for user to hit SELECT to return to title screen
        waitForVBlank();
        drawFullScreenImageDMA(loseimage);
        drawCenteredString(140, 120, 0, 0, "Press SELECT to return to start menu...", WHITE);
        if (((~currentButtons & BUTTON_SELECT) == 0) && ((~previousButtons & BUTTON_SELECT) != 0)) {
          state = START;
        }
        break;
    }

    previousButtons = currentButtons;  // Store the current state of the buttons
  }


  return 0;
}
