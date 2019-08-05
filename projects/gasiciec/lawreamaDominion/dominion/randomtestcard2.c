/******************************************************************************
* * PROGRAM NAME:   randomtestcard2.c
* * AUTHOR:         Clarissa Gasiciel
* * DATE MODIFIED:  August 4, 2019
* * DESCRIPTION:    Random test for Minion card functionality (updated to work
* *                 with classmate code)
******************************************************************************/
#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "rngs.h"
#include "testHelper.h"
#include <math.h>

void checkMinion(int choice1, int choice2, int handPos, struct gameState *post, int currPlayer)
{
  //Iterators
  int i;

  //Declare control gameState
  struct gameState pre;
  memcpy(&pre, post, sizeof(struct gameState));

  do_minion(choice1, choice2, handPos, post, currPlayer);

  //Adjust control gameState to expected values
  pre.numActions++;

  //Discard minion card
  discardCard(handPos, currPlayer, &pre, 0);


  //Player chooses to gain 2 coins
  if(choice1)
  {
    pre.coins += 2;
  }

  //Player chooses to discard hand and redraw
  else
  {
      printf("Updating counts\n");
      //current player discards and all players with >= 5 cards discard and draw 4
      for(i = 0; i < pre.numPlayers; i++)
      {
        if(i == currPlayer || pre.handCount[i] >= 5)
        {
            //Update discard count
            pre.discardCount[i] += pre.handCount[i];

            //Update hand count
            pre.handCount[i] = 4;

            //Update deck count
            //Deck has at least 4 cars
            if(pre.deckCount[i] >= 4)
            {
                pre.deckCount[i] -= 4;
            }

            //Discard was added to deck because not enough cards
            else
            {
              pre.deckCount[i] += pre.discardCount[i];
            }
        }
      }
  }

  //Compare states
  printf("+1 added to Actions\n");
  assertTrue(pre.numActions == post->numActions);


  printf("    *******  Coins at expected value    *******  \n");
  assertTrue(pre.coins == post->coins);

  //Compare hand, discard, and deck counts
  for(i = 0; i < pre.numPlayers; i++)
  {
    printf("    *******  Player %d hand count at expected value    *******  \n", i + 1);
    assertTrue(pre.handCount[i] == post->handCount[i]);

    printf("    *******  Player %d discard count at expected value    *******  \n", i + 1);
    assertTrue(pre.discardCount[i] == post->discardCount[i]);

    printf("    *******  Player %d deck count at expected value    *******  \n", i + 1);
    assertTrue(pre.discardCount[i] == post->discardCount[i]);
  }
}




int main()
{
  //Iterators for for loops
  int i, j;

  //Array of kingdom cards for game
  int kingdomCards[10] = {adventurer, council_room, feast, gardens, mine,
      remodel, smithy, minion, baron, great_hall};


  struct gameState TestGame;

  printf("   **********************************************************************************    \n");
  printf("   ****************************    TESTING playMinion()   ****************************    \n");
  printf("   **********************************************************************************    \n\n");

  //Set up random number generator
  SelectStream(1);
  PutSeed(-3);

  int tests = 50;
  int testCounter = 0;

  while(testCounter < tests)
  {
    printf("   ****************************    TEST %d   ****************************    \n\n", testCounter + 1);
    //Fill state with random data to avoid calling on initializeGame()
    //Source:  testDrawCard.c
    for(i = 0; i < 2000; i++)
    {
      for(j = 0; j < sizeof(struct gameState); j++)
      {
        ((char*)&TestGame)[i] = floor(Random() * 256);
      }
    }

    //Set key data members to "sane" data
    //Randomize number of players (between 2 and 4)
    int players = floor(Random() * 4);
    if(players < 2)
    {
      players += 2;
    }

    TestGame.numPlayers = players;
    printf("Number of players: %d\n\n", TestGame.numPlayers);


    //Set current player to random player
    int currPlayer = floor(Random() * TestGame.numPlayers);
    printf("Current Player: %d\n\n", currPlayer);


    //Set number of Actions = 1 as each turn should start this way
    TestGame.numActions = 1;


    //Set number of coins
    int maxCoins = 130;
    TestGame.coins = floor(Random() * maxCoins);
    printf("Coins:  %d\n\n", TestGame.coins);


    //Set number of Kingdom Cards
    for(i = adventurer; i <= treasure_map; i++)
    {
      for(j = 0; j < 10; j++)
      {
        if(kingdomCards[j] == i)
        {
          //check if card is a 'Victory' Kingdom card
          if (kingdomCards[j] == great_hall || kingdomCards[j] == gardens)
          {
            if (players == 2){
              TestGame.supplyCount[i] = floor(Random() * 8);
            }

            else
            {
              TestGame.supplyCount[i] = floor(Random() * 12);
            }
          }

          //Card is not a Victory card
          else
          {
            TestGame.supplyCount[i] = floor(Random() * 10);
          }
            break;
          }

          //card is not in the set choosen for the game
          else
          {
            TestGame.supplyCount[i] = -1;
          }
        }
      }


      //Randomize supply of Victory cards
      if (players == 2)
      {
        TestGame.supplyCount[estate] = floor(Random() * 8);
        TestGame.supplyCount[duchy] = floor(Random() * 8);
        TestGame.supplyCount[province] = floor(Random() * 8);
      }
      else
      {
        TestGame.supplyCount[estate] = floor(Random() * 12);
        TestGame.supplyCount[duchy] = floor(Random() * 12);
        TestGame.supplyCount[province] = floor(Random() * 12);
      }





    //Randomize hand and discard count for all players
    for(i = 0; i < TestGame.numPlayers; i++)
    {
      int cardMod = 0;

      TestGame.handCount[i] = floor(Random() * MAX_HAND);
      //Calculate how many cards left to fill other piles
      cardMod = MAX_HAND - TestGame.handCount[i];
      printf("Player %d hand count is: %d\n\n", i + 1, TestGame.handCount[i]);
      randomizeHand(&TestGame, i);

      TestGame.deckCount[i] = floor(Random() * cardMod);
      cardMod -= TestGame.deckCount[i];
      printf("Player %d deck count: %d\n\n", i + 1, TestGame.deckCount[i]);
      randomizeDeck(&TestGame, i);

      TestGame.discardCount[i] = floor(Random() * cardMod);
      printf("Player %d discard count: %d\n\n", i + 1, TestGame.discardCount[i]);
      randomizeDiscard(&TestGame, i);
    }

    //Randomize if choice 1 or 2
    int choice1 = floor(Random() * 2);

    int choice2 = 0;

    //If choice1 is false, pick choice2
    if(!choice1)
    {
      choice2 = 1;
    }

    printf("choice1 is: %d\n\n", choice1);
    printf("choice2 is: %d\n\n", choice2);

    //Choose random card in hand to be minion
    int handPos = (floor(Random() * TestGame.handCount[currPlayer]) - 1);


    //Call minion test
    checkMinion(choice1, choice2, handPos, &TestGame, currPlayer);


    testCounter++;
  }

  return 0;
}
