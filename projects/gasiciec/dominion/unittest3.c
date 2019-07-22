/******************************************************************************
* * PROGRAM NAME:   unittest3.c
* * AUTHOR:         Clarissa Gasiciel
* * DATE MODIFIED:  July 21, 2019
* * DESCRIPTION:    Unit test for Ambassador card functionality
* *                 Modified from cardtest4.c
******************************************************************************/
#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "rngs.h"
#include "testHelper.h"

#define TESTCARD "ambassador"


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

  //Choose Kingdom cards for supply
  int k[10] = {adventurer, council_room, feast, gardens, mine, remodel, smithy, village, ambassador, great_hall};

  //Initialize parameters for cardEffect
  int choice1 = 0, choice2 = 0;
  int handPos = 5;

  int i;

  //Initialize game statement
  initializeGame(numPlayers, k, seed, &game);


  //Populate player hands
  for(i = 0; i < numPlayers; i++)
  {
     game.hand[i][0] = adventurer;
     game.hand[i][1] = copper;
     game.hand[i][2] = estate;
     game.hand[i][3] = mine;
     game.hand[i][4] = copper;
     game.hand[i][5] = ambassador;

     game.handCount[i] = 6;
  }

  printf(" -----  TESTING %s  ----- \n", TESTCARD);

  //Test for
  printf(" -- TEST 1: Player reveals card, chooses to return 0 -- \n");

  //Pick card to reveal
  choice1 = 1;

  //Copy game state to test case
  memcpy(&test, &game, sizeof(struct gameState));


  //Call cardEffect
  int result = playAmbassador(choice1, choice2, &test, handPos, thisPlayer);

  //Check that no cards have been discarded other than Ambassador
  printf("Hand count: %d, expected: %d\n", test.handCount[thisPlayer], game.handCount[thisPlayer] - 1);
  assertTrue(test.handCount[thisPlayer] == game.handCount[thisPlayer] - 1);


  //Test returning 1 copy of revealed card
  printf(" -- TEST 2: Player reveals card, chooses to return 1 -- \n");
  //Set choice to 1
  choice2 = 1;

  //Copy game state to test case
  memcpy(&test, &game, sizeof(struct gameState));

  //Call cardEffect
  result = playAmbassador(choice1, choice2, &test, handPos, thisPlayer);

  //Check that only Ambassador and 1 other card discarded
  printf("Hand count: %d, expected: %d\n", test.handCount[thisPlayer], game.handCount[thisPlayer] - 2);
  assertTrue(test.handCount[thisPlayer] == game.handCount[thisPlayer] - 2);



  //Run Test 3
  printf(" -- TEST 3: Player reveals card, chooses to return 2 -- \n");
  choice2 = 2;

  //Copy game state to test case
  memcpy(&test, &game, sizeof(struct gameState));


  //Call cardEffect
  result = playAmbassador(choice1, choice2, &test, handPos, thisPlayer);

  //Check that Ambassador has been added to played
  printf("Played card count: %d, expected: %d\n", test.playedCardCount, game.playedCardCount + 1);
  assertTrue(test.playedCardCount == game.playedCardCount + 1);

  //Check that Ambassador and two cards have been removed from hand
  printf("Hand count: %d, expected: %d\n", test.handCount[thisPlayer], game.handCount[thisPlayer] - 2);
  assertTrue(test.handCount[thisPlayer] == game.handCount[thisPlayer] - 2);



  printf(" -- TEST 4: All other players get copies of chosen card -- \n");
  i = numPlayers;
  int currPlayer = 0;
  int playerCounter = 0;

  //Set choice to 1
  choice2 = 1;

  //Copy game state to test case
  memcpy(&test, &game, sizeof(struct gameState));

  //Call cardEffect
  result = playAmbassador(choice1, choice2, &test, handPos, thisPlayer);

  //Check that all other players have copies of the revealed card
  while(i)
  {
    if(currPlayer != thisPlayer && test.discard[currPlayer][test.discardCount[currPlayer]-1] == copper)
    {
        playerCounter++;
    }

    currPlayer++;
    i--;
  }

  assertTrue(playerCounter == numPlayers - 1);



  printf(" -- TEST 5: Player tries to return more than 2 copies of revealed card -- \n");
  choice2 = 5;

  //Copy game state to test case
  memcpy(&test, &game, sizeof(struct gameState));

  //Call cardEffect
  result = playAmbassador(choice1, choice2, &test, handPos, thisPlayer);

  //Check that user
  printf("Choosing more than 2 cards does not work or crash the game\n");
  assertTrue(result == -1);


  printf(" -- TEST 6: Player tries to return < 0 cards -- \n");
  choice2 = -10;

  //Copy game state to test case
  memcpy(&test, &game, sizeof(struct gameState));

  //Call cardEffect
  result = playAmbassador(choice1, choice2, &test, handPos, thisPlayer);

  //Check that user
  printf("Choosing less than 0 cards does not work or crash the game\n");
  assertTrue(result == -1);

  return 0;
}
