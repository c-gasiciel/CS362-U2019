/******************************************************************************
* * PROGRAM NAME:   unittest1.c
* * AUTHOR:         Clarissa Gasiciel
* * DATE MODIFIED:  July 21, 2019
* * DESCRIPTION:    Unit test for Baron card functionality
* *                 Modified from cardtest4.c
******************************************************************************/
#include "dominion.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "rngs.h"
#include "testHelper.h"

#define TESTCARD "baron"


int main()
{
  //Declare game struct
  struct gameState game;

  //Declare test game struct
  struct gameState test;

  //Seed random number generator
  int seed = 1000;
  //Set number of players
  int numPlayers = 2;
  //Initialize to first player
  int thisPlayer = 0;

  //Init buy increase
  int buyIncrease = 1;

  //Init coin increase
  int coinIncrease = 4;

  //Choose Kingdom cards for supply
  int k[10] = {adventurer, council_room, feast, gardens, mine, remodel, smithy, village, baron, great_hall};

  //Iterator
  int i;

  //Initialize parameters for cardEffect
  int choice1 = 0, choice2 = 0, choice3 = 0;
  int handPos = 0, bonus = 0;


  //Initialize game statement
  initializeGame(numPlayers, k, seed, &game);

  printf(" -----  TESTING %s  ----- \n", TESTCARD);

  //Test for increase in buy count
  printf(" -- TEST 1: Buy +1 -- \n");

  //Copy game state to test case
  memcpy(&test, &game, sizeof(struct gameState));

  //Call cardEffect
  cardEffect(baron, choice1, choice2, choice3, &test, handPos, &bonus);

  printf("Buy count: %d, expected: %d\n", test.numBuys, game.numBuys + buyIncrease);
  assertTrue(test.numBuys == game.numBuys + buyIncrease);


  //Test for discarding estate with estate in hand
  printf(" -- TEST 2: choice1 = 1, estate to discard -- \n");
  //Set choice to 1
  choice1 = 1;

  //Copy game state to test case
  memcpy(&test, &game, sizeof(struct gameState));

  //Make sure there's an estate in the player's hand to discard
  int cardPos;

  int hasCard = searchHand(&test, estate, thisPlayer, &cardPos);

  //If no estate present, add estate to player's hand
  if(!hasCard)
  {
    test.hand[thisPlayer][test.handCount[thisPlayer] - 1] = estate;
  }


  //Call cardEffect()
  cardEffect(baron, choice1, choice2, choice3, &test, handPos, &bonus);

  //Check that player has gained 4 coins
  printf("Coin count: %d, expected: %d\n", test.coins, game.coins + coinIncrease);
  assertTrue(test.coins == game.coins + coinIncrease);

  //Check that estate has been discarded
  printf("Check that estate has been discarded");
  assertTrue(test.hand[thisPlayer][test.handCount[thisPlayer] - 1] == -1);


  printf(" -- TEST 3: choice1 = 1, no estate to discard -- \n");

  printf(" -- TEST 4: choice1 = 0, +1 estate -- \n");

  return 0;
}
