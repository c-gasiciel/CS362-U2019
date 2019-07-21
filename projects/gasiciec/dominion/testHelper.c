/******************************************************************************
* * PROGRAM NAME:   testHelper.c
* * AUTHOR:         Clarissa Gasiciel
* * DATE MODIFIED:  July 21, 2019
* * DESCRIPTION:    Helper functions for test suit
******************************************************************************/
#include "testHelper.h"

/******************************************************************************
* * DESCRIPTION:  assertTrue() -- checks if test has been passed
******************************************************************************/
void assertTrue(int isTrue)
{
  if(isTrue)
  {
      printf("Passed\n");
  }

  else
  {
    printf("Failed\n");
  }
}





/******************************************************************************
* * DESCRIPTION:  searchHand() -- searches the player's hand for a particular
* *               card; returns true if that card is found in the player's hand
*****************************************************************************/
int searchHand(struct gameState *state, CARD val, int currPlayer, int *pos)
{
  int i;

  for(i = 0; i < state->handCount[currPlayer]; i++)
  {
    //If card found, record position and return true
    if(state->hand[currPlayer][i] == val);
    {
      *pos = i;
      return 1;
    }
  }

  //Card not found; return false
  return 0;
}
