#include "play.h"
#include "gba.h"
#include "game.h"

//image files
#include "images/yellowjacket.h"
#include "images/heart.h"
#include "images/emptyheart.h"
#include "images/dawghead.h"
#include "images/buzzhead.h"
#include "images/dawg.h"
#include "images/football.h"

//this function is the play function of the program and holds the main game loop
int play(void) {

    //initialize play screen elements

    waitForVBlank();
    //EMPTY SCREEN
    fillScreenDMA(BLACK);
    waitForVBlank();
    //dawg health tile
    drawRectDMA(143, 110, 104, 12, GRAY);
    //buzz icon
    drawImageDMA(139, 0, 20, 20, buzzhead);
    //dawg icon
    drawImageDMA(139, 219, 20, 20, dawghead);
    //dawg health bar
    drawRectDMA(145, 112, 100, 8, GREEN);
    //hearts
    drawImageDMA(139, 20, 20, 20, heart);
    drawImageDMA(139, 40, 20, 20, heart);
    drawImageDMA(139, 60, 20, 20, heart);
    //bottom text
    drawString(130, 2, "Buzz Lives: ", WHITE);
    drawString(130, 108, "Bulldog Health:100%", WHITE);
    

    //initialize variables
    int bulldoghealth = 100;
    int buzzlives = 3;

    //create objects
    u32 previousButtons = BUTTONS;
    u32 currentButtons = BUTTONS;
    Obj cbuzz;
    Obj pbuzz;
    Obj cdawg;
    Obj pdawg;
    Obj cfball;
    Obj pfball;
    Obj cball;
    Obj pball;

    //initialize buzz, bulldog, fball, and ball components
    cbuzz.row = 80;
    cbuzz.col = 10;
    cdawg.row = 80;
    cdawg.col = 219;
    cdawg.vely = -1;
    cfball.row = cdawg.row + 5;
    cfball.col = cdawg.col - 20;
    cfball.velx = -3;
    cball.row = cbuzz.row + 12;
    cball.col = cbuzz.col + 17;
    cball.velx = 0;



    //this is the main game loop which updates the screen elements based on user input. The loop escaped when buzz or dawg have 0 health, or the select key is pressed.
    //In each case, an appropriate value is returned.
    while (1) {

        //set current version of objects and buttons to previous version
        previousButtons = currentButtons;
        pbuzz = cbuzz;
        pdawg = cdawg;
        pball = cball;
        pfball = cfball;

        //return to start menu if player clicks select
        currentButtons = BUTTONS;
        if (((~currentButtons & BUTTON_SELECT) == 0) && ((~previousButtons & BUTTON_SELECT) != 0)) {
            return 0;
        }

        
        //buzz movement math based on arrow keys
        if ((~currentButtons & BUTTON_UP) != 0 && cbuzz.row > 1) {
            cbuzz.row -= 2;
        }
        if ((~currentButtons & BUTTON_DOWN) != 0 && cbuzz.row < 110) {
            cbuzz.row += 2;
        }
        if ((~currentButtons & BUTTON_LEFT) != 0 && cbuzz.col > 1) {
            cbuzz.col -= 2;
        }
        if ((~currentButtons & BUTTON_RIGHT) != 0 && cbuzz.col < 177) {
            cbuzz.col += 2;
        }   
    
        //bulldog movement math
        if (cdawg.row == 0) {
            cdawg.vely = 1;
        }
        if (cdawg.row == 110) {
            cdawg.vely = -1;
        }
        cdawg.row += cdawg.vely;

        
        //football movement math
        //buzz hitbox
        int colspace = cfball.col - cbuzz.col;
        //horizontal hitbox check
        if (colspace < 20 && colspace > -5) {
            int rowspace = cfball.row - cbuzz.row;
            //vertical hitbox check
            if (rowspace < 20 && rowspace > -12) {
                //buzz hit functionality
                cfball.row = cdawg.row + 5;
                cfball.col = cdawg.col - 20;
                buzzlives--;
                //end game as a loss if buzz health is 0
                if (buzzlives == 0) {
                   return 2;
                }
            }
        }
        //leftwall boundary to return football to dawg
        if (cfball.col < 3) {
            cfball.row = cdawg.row + 5;
            cfball.col = cdawg.col - 20;
        }
        cfball.col += cfball.velx;


        //ball movement math
        //buldog hitbox
        int colspace2 = cball.col - cdawg.col;
        //horizontal hitbox check
        if (colspace2 < 17 && colspace2 > -3) {
            int rowspace2 = cball.row - cdawg.row;
            //vertical hitbox check
            if (rowspace2 < 20 && rowspace2 > -3) {
                //bulldog hit functionality
                cball.row = cbuzz.row + 12;
                cball.col = cbuzz.col + 17;
                cball.velx = 0;
                bulldoghealth -= 10;
                //end game as a win if bulldog health is 0
                if (bulldoghealth == 0) {
                    return 1;
                }
            }
        }
        //rightwall boundary to return ball to buzz
        if (cball.col > 234) {
            cball.row = cbuzz.row + 12;
            cball.col = cbuzz.col + 17;
            cball.velx = 0;
        }
        //fire ball on the click of A
        if ((~currentButtons & BUTTON_A) != 0) {
            cball.velx = 3;
        }
        if (cball.velx == 0) {
          cball.row = cbuzz.row + 12;
          cball.col = cbuzz.col + 17;
        }
        cball.col += cball.velx;   


        
        //wait for VBlank and update all the screen elements
        waitForVBlank();
        //Erase old buzz
        drawRectDMA(pbuzz.row, pbuzz.col, 20, 20, BLACK);
        //Draw new buzz
        drawImageDMA(cbuzz.row, cbuzz.col, 20, 20, yellowjacket);
        //Erase old dawg
        drawRectDMA(pdawg.row, pdawg.col, 20, 20, BLACK);
        //Draw new dawg
        drawImageDMA(cdawg.row, cdawg.col, 20, 20, dawg);
        //Erase old fball
        drawRectDMA(pfball.row, pfball.col, 20, 12, BLACK);
        //Draw new fball
        drawImageDMA(cfball.row, cfball.col, 20, 12, football);
        //Erase old ball
        drawRectDMA(pball.row, pball.col, 3, 3, BLACK);
        //Draw new ball
        drawRectDMA(cball.row, cball.col, 3, 3, WHITE);


        //Update Buzz hearts
        if (buzzlives == 2) {
            drawImageDMA(139, 60, 20, 20, emptyheart);       
        }
        else if (buzzlives == 1) {
            drawImageDMA(139, 40, 20, 20, emptyheart);
        }
        //update Buzz lives text
        drawRectDMA(130, 70, 6, 8, BLACK);
        drawChar(130, 70, (buzzlives + 48), WHITE);


        //Update Bulldog health bar with appropriate coloring for health bar
        drawRectDMA(145, 112, 100, 8, BLACK);
        if (bulldoghealth >= 70) {
            drawRectDMA(145, 112, bulldoghealth, 8, GREEN);
        }
        else if (bulldoghealth >= 40) {
            drawRectDMA(145, 112, bulldoghealth, 8, YELLOW);
        }
        else {
            drawRectDMA(145, 112, bulldoghealth, 8, RED);    
        }
        //Update bulldog health text
        if (bulldoghealth != 100) {
            drawRectDMA(130, 198, 24, 8, BLACK);
            drawChar(130, 198, bulldoghealth/10 + 48, WHITE);
            drawString(130, 204, "0%", WHITE);
        }


    } //end of game loop. This place is unreachable



}