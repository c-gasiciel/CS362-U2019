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
int assertTrue(int isTrue)
{
  if(isTrue)
  {
      printf("Passed\n\n");
      return 1;
  }

  else
  {
    printf("Failed\n\n");
    return 0;
  }
}





/******************************************************************************
* * DESCRIPTION:  searchHand() -- searches the player's hand for a particular
* *               card; returns true if that card is found in the player's hand
*****************************************************************************/
int searchHand(struct gameState *state, int val, int currPlayer, int *pos)
{
  int i;

  for(i = 0; i < state->handCount[currPlayer]; i++)
  {
    //If card found, record position and return true
    if(state->hand[currPlayer][i] == val)
    {
      *pos = i;
      return 1;
    }
  }

  //Card not found; return false
  return 0;
}



/******************************************************************************
* * DESCRIPTION:  searchDiscard() -- searches the player's discard for a particular
* *               card; returns true if that card is found
*****************************************************************************/
int searchDiscard(struct gameState *state, int val, int currPlayer, int *pos)
{
  int i;

  for(i = 0; i < state->discardCount[currPlayer]; i++)
  {
    //If card found, record position and return true
    if(state->discard[currPlayer][i] == val);
    {
      *pos = i;
      return 1;
    }
  }

  //Card not found; return false
  return 0;
}





/******************************************************************************
* * DESCRIPTION:  removeCards() -- remove all cards of a certain type from
* *               player's hand
*****************************************************************************/
void removeCards(struct gameState *state, int rmVal, int currPlayer, int rpVal)
{
  int i;

  //Go through hand
  for(i = 0; i < state->handCount[currPlayer]; i++)
  {
    //If card is found, replace with different card
    if(state->hand[currPlayer][i] == rmVal)
    {
      state->hand[currPlayer][i] = rpVal;
    }
  }
}
