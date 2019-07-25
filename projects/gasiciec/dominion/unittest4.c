/******************************************************************************
* * PROGRAM NAME:   unittest2.c
* * AUTHOR:         Clarissa Gasiciel
* * DATE MODIFIED:  July 21, 2019
* * DESCRIPTION:    Unit test for Tribute card functionality
* *                 Modified from cardtest4.c
******************************************************************************/
#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "rngs.h"
#include "testHelper.h"

#define TESTCARD "Tribute"


int main()
{
  //Declare game struct
  struct gameState game;

  //Declare test game struct
  struct gameState test;

  //Seed random number generator
  int seed = 1000;
  //Set number of players
  int numPlayers = 3;
  //Initialize to first player
  int thisPlayer = 0;

  //Init action increase
  int actionIncrease = 1;

  //Init coin increase
  int coinIncrease = 2;

  //Init number of cards to draw
  //int drawCount = 4;

  //Choose Kingdom cards for supply
  int k[10] = {adventurer, council_room, feast, gardens, mine, remodel, smithy, village, baron, great_hall};

  //Initialize parameters for cardEffect
  int choice1 = 0, choice2 = 0, choice3 = 0;
  int handPos = 0, bonus = 0;

  int i;

  //Initialize game statement
  initializeGame(numPlayers, k, seed, &game);

  printf(" -----  TESTING %s  ----- \n", TESTCARD);

  //Test for increase in action count
  printf(" -- TEST 1: Action +1 -- \n");

  //Copy game state to test case
  memcpy(&test, &game, sizeof(struct gameState));

  //Call cardEffect
  cardEffect(minion, choice1, choice2, choice3, &test, handPos, &bonus);

  printf("Action count: %d, expected: %d\n", test.numActions, game.numActions + actionIncrease);
  assertTrue(test.numActions == game.numActions + actionIncrease);


  //Test for gaining two coins
  printf(" -- TEST 2: choice1 = 1, gain two coins -- \n");
  //Set choice to 1
  choice1 = 1;

  //Copy game state to test case
  memcpy(&test, &game, sizeof(struct gameState));

  //Call cardEffect()
  cardEffect(minion, choice1, choice2, choice3, &test, handPos, &bonus);

  //Check that player has gained 2 coins
  printf("Coin count: %d, expected: %d\n", test.coins, game.coins + coinIncrease);
  assertTrue(test.coins == game.coins + coinIncrease);


  //Run Test 3
  printf(" -- TEST 3: choice1 = 0, discard hand and draw 4 -- \n");
  choice1 = 0;

  //Copy game state to test case
  memcpy(&test, &game, sizeof(struct gameState));

  //If Player 2 doesn't have 5 cards, give them 5 cards
  if(test.handCount[thisPlayer + 1] < 5)
  {
    for(i = test.handCount[thisPlayer + 1]; i < 5; i++)
    {
        test.hand[thisPlayer + 1][i] = gardens;
        printf("Added card to Player 2's hand\n");
    }
  }


  //If Player 3 has more than 5 cards, decrease hand size
  int difference = test.handCount[thisPlayer +2] - 4;

  if(difference > 0)
  {
    for(i = 0; i < difference; i++)
    {
      discardCard(i, thisPlayer + 2, &test, 0);
    }
  }



  //Call cardEffect
  cardEffect(minion, choice1, choice2, choice3, &test, handPos, &bonus);

  printf("Current Player's hand was discarded\n");
  printf("Discard count: %d, expected: %d\n", test.discardCount[thisPlayer], game.handCount[thisPlayer]);
  assertTrue(test.discardCount[thisPlayer] == game.handCount[thisPlayer]);

  printf("Current Player has four cards in hand\n");
  assertTrue(test.handCount[thisPlayer] == 4);

  printf("Players with 5 or more cards discarded their hand\n");
  i = numPlayers;
  int currPlayer = 0;
  int discardPlayerCounter = 0;
  int discardMatchCounter = 0;

  while(i)
  {
    if(game.handCount[currPlayer] > 4 && currPlayer != thisPlayer)
    {
        discardPlayerCounter++;
        if(test.discardCount[currPlayer] == game.handCount[currPlayer])
        {
          discardMatchCounter++;
        }
    }

    currPlayer++;
    i--;
  }

  assertTrue(discardPlayerCounter == discardMatchCounter);


  printf("Other Players who discarded have 4 cards in hand\n");
  i = 0;
  currPlayer = 0;
  discardPlayerCounter = 0;
  discardMatchCounter = 0;

  while(i)
  {
    if(game.handCount[currPlayer] > 4 && currPlayer != thisPlayer)
    {
        discardPlayerCounter++;
        if(test.handCount[currPlayer] == 4)
        {
          discardMatchCounter++;
        }
    }

    currPlayer++;
    i--;
  }

  assertTrue(discardPlayerCounter == discardMatchCounter);

  printf("Players with less than 5 cards have unchanged hands\n");
  i = 0;
  currPlayer = 0;
  discardPlayerCounter = 0;
  discardMatchCounter = 0;

  while(i)
  {
    if(game.handCount[currPlayer] < 5 && currPlayer != thisPlayer)
    {
        discardPlayerCounter++;
        int j;
        int isSame = 1;

        for(j = 0; j < game.handCount[currPlayer]; j++)
        {
            if(game.hand[currPlayer][j] != test.hand[currPlayer][j])
            {
              isSame = 0;
              break;
            }
        }

        if(isSame)
        {
          discardMatchCounter++;
        }
    }

    currPlayer++;
    i--;
  }

  assertTrue(discardPlayerCounter == discardMatchCounter);


  return 0;
}
