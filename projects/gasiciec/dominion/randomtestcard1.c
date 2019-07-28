/******************************************************************************
* * PROGRAM NAME:   randomtestcard1.c
* * AUTHOR:         Clarissa Gasiciel
* * DATE MODIFIED:  July 27, 2019
* * DESCRIPTION:    Random test for Baron card functionality
* *                 Adapted from testDrawCard.c
******************************************************************************/
#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "rngs.h"
#include "testHelper.h"
#include <math.h>


//Method to test playBaron()
void checkBaron(int choice1, int currPlayer, struct gameState *post, int firstEstate ,int hasEstate)
{
  //Iterators
  int j;

  //Declare control gameState
  struct gameState pre;
  memcpy(&pre, post, sizeof(struct gameState));

  //Call playBaron()
  playBaron(choice1, currPlayer, post);


  //Adjust control gameState to expected values
  //Increase number of buys by 1
  pre.numBuys++;


  //choice1 > 0, discard estate
  if(choice1 > 0)
  {
      if(hasEstate)
      {
        //Add 4 coins
        pre.coins += 4;

        //Discard estate
        pre.discard[currPlayer][pre.discardCount[currPlayer]] = pre.hand[currPlayer][firstEstate];
        pre.discardCount[currPlayer]++;

        //Remove estate from hand
        for(j = firstEstate; j < pre.handCount[currPlayer]; j++)
        {
          pre.hand[currPlayer][j] = pre.hand[currPlayer][j + 1];
        }

        //Set last card to invalid value
        pre.hand[currPlayer][pre.handCount[currPlayer - 1]] = -1;

        //Decrement hand count
        pre.handCount[currPlayer]--;
      }

      //Player does not have an estate to discard and estates available
      else if(!hasEstate && pre.supplyCount[estate] > 0)
      {
        //Player should have an estate in discard
        pre.discard[currPlayer][pre.discardCount[currPlayer]] = estate;
        //Discard count should increase
        pre.discardCount[currPlayer]++;

        //Supply count for estate should decrease
        pre.supplyCount[estate]--;
      }

  }


  //choice1 = 0, gain estate if available
  else
  {
    if(pre.supplyCount[estate] > 0)
    {
      //Player should have an estate in discard
      pre.discard[currPlayer][pre.discardCount[currPlayer]] = estate;
      //Discard count should increase
      pre.discardCount[currPlayer]++;

      //Supply count for estate should decrease
      pre.supplyCount[estate]--;
    }
  }


  //Compare control state (pre) and test state (post)
  printf("    *******  Comparing test state and control state  *******    \n");
  if(!assertTrue(memcmp(&pre, post, sizeof(struct gameState)) == 0))
  {
    //Run additional comparisons if states are different
    printf("    *******  Checking number of buys  *******   \n");
    assertTrue(pre.numBuys == post->numBuys);

    printf("    *******  Checking hand count  *******   \n");
    assertTrue(pre.handCount[currPlayer] == post->handCount[currPlayer]);

    printf("    *******  Checking for removal of estate from hand  *******    \n");
    assertTrue(pre.hand[currPlayer][firstEstate] == post->hand[currPlayer][firstEstate]);

    printf("    *******  Comparing discard count  *******   \n");
    assertTrue(pre.discardCount[currPlayer] == post->discardCount[currPlayer]);

    printf("    *******  Comparing top discarded card  *******    \n");
    assertTrue(pre.discard[currPlayer][pre.discardCount[currPlayer] - 1] == post->discard[currPlayer][post->discardCount[currPlayer] - 1]);

    printf("    *******  Comparing estate supply count  *******   \n");
    assertTrue(pre.supplyCount[estate] == post->supplyCount[estate]);

    printf("    *******  Comparing number of coins  *******   \n");
    assertTrue(pre.coins == post->coins);
  }
}




int main()
{
  //Iterators for for loops
  int i, j;

  //Array of kingdom cards for game
  int kingdomCards[10] = {adventurer, council_room, feast, gardens, mine,
      remodel, smithy, village, baron, great_hall};


  struct gameState TestGame;

  printf("   **********************************************************************************    \n");
  printf("   ****************************    TESTING playBaron()   ****************************    \n");
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
    printf("Players: %d\n\n", TestGame.numPlayers);

    //Set current player to random player
    int currPlayer = floor(Random() * TestGame.numPlayers);
    //printf("Current Player: %d\n\n", currPlayer);


    //Set number of Actions = 1 as each turn should start this way
    TestGame.numActions = 1;


    //Set number of buys to 1 as each turn should start this way
    TestGame.numBuys = 1;


    //Set number of coins
    //SelectStream(0);
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

      printf("Estate count: %d\n\n", TestGame.supplyCount[estate]);


      //Set player deck
      TestGame.deckCount[currPlayer] = floor(Random() * MAX_DECK);
      printf("Deck count: %d\n\n", TestGame.deckCount[currPlayer]);


      //randomize hand for current player
      TestGame.handCount[currPlayer] = floor(Random() * MAX_HAND);
      printf("Hand count is: %d\n\n", TestGame.handCount[currPlayer]);

      for(i = 0; i < TestGame.handCount[currPlayer]; i++)
      {
        int isAvailable = 0;

        while(!isAvailable)
        {
          //Pick random card
          int pickCard = floor(Random() * treasure_map);

          //If card is in game, add to hand
          if(TestGame.supplyCount[pickCard] != -1)
          {
            TestGame.hand[currPlayer][i] = pickCard;
            isAvailable = 1;
          }
        }
      }


    //Find first estate if one exists
    int firstEstate;
    int foundEstate = searchHand(&TestGame, estate, currPlayer, &firstEstate);
    /*if(foundEstate)
    {
        printf("Position of first Estate: %d\n\n", firstEstate);
    }

    else
    {
      printf("No estates in hand\n\n");
    } */


    //Randomize discard count
    TestGame.discardCount[currPlayer] = floor(Random() * MAX_DECK);
    printf("Discard count: %d\n\n", TestGame.discardCount[currPlayer]);



    //Randomize choice1 (0 or 1)
    int choice1 = floor(Random() * 2);
    printf("choice1 is: %d\n\n", choice1);


    //Call checkBaron()
    checkBaron(choice1, currPlayer, &TestGame, firstEstate, foundEstate);

    testCounter++;
  }

  printf(" -----  TESTING COMPLETE  ----- \n\n");

  return 0;
}
