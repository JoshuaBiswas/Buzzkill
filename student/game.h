#ifndef GAME_H
#define GAME_H

#include "gba.h"

//Object struct to be used for moving game elements
typedef struct obj {
        //row and column location for the object
        int row;
        int col;
        //horizontal and vertical velocity for the object
        int velx;
        int vely;
    } Obj;

/*
* For example, for a Snake game, one could be:
*
* typedef struct snake {
*   int heading;
*   int length;
*   int row;
*   int col;
* } Snake;
*
*
* 
*
*
* Example of a struct to hold state machine data:
* 
* typedef struct state {
*   int currentState;
*   int nextState;
* } State
*
*/

#endif
