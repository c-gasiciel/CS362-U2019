/******************************************************************************
* * PROGRAM NAME:   testHelper.h
* * AUTHOR:         Clarissa Gasiciel
* * DATE MODIFIED:  July 21, 2019
* * DESCRIPTION:    Header file for test suit helper functions
******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include "dominion.h"

void assertTrue(int isTrue);
int searchHand(struct gameState *state, int val, int currPlayer, int *pos);
void removeCards(struct gameState *state, int rmVal, int currPlayer, int rpVal);
int searchDiscard(struct gameState *state, int val, int currPlayer, int *pos);
