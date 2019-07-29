/******************************************************************************
* * PROGRAM NAME:   randomtestcard3.c
* * AUTHOR:         Clarissa Gasiciel
* * DATE MODIFIED:  July 27, 2019
* * DESCRIPTION:    Random test for Tribute card functionality
******************************************************************************/
#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "rngs.h"
#include "testHelper.h"
#include <math.h>


void checkTribute(struct gameState *post, int nextPlayer, int currPlayer)
{
  //Iterators
  int i;

  //Revealed card array
  int tributeRevealedCards[2] = {-1, -1};


  //Declare control gameState
  struct gameState pre;
  memcpy(&pre, post, sizeof(struct gameState));

  playTribute(post, nextPlayer, currPlayer);

  //Adjust control to expected values
  //Check next player's deck for 1 or fewer cards
  if(pre.deckCount[nextPlayer] <= 1)
  {
    if(pre.deckCount[nextPlayer] > 0)
    {
      tributeRevealedCards[0] = pre.deck[nextPlayer][pre.deckCount[nextPlayer] - 1];
      pre.deckCount[nextPlayer]--;
    }

    //Take from discard
    else
    {
      tributeRevealedCards[0] = pre.discard[nextPlayer][pre.discardCount[nextPlayer] - 1];
      pre.discardCount[nextPlayer]--;
    }
  }

  else
  {
    //If deck is empty
    if(pre.deckCount[nextPlayer] == 0)
    {
      moveDiscardToDeck(&pre, nextPlayer);
    }

    tributeRevealedCards[0] = pre.deck[nextPlayer][pre.deckCount[nextPlayer] - 1];
    //Remove card from deck
    pre.deck[nextPlayer][pre.deckCount[nextPlayer] - 1] = -1;
    //Decrement deck count
    pre.deckCount[nextPlayer]--;


    tributeRevealedCards[1] = pre.deck[nextPlayer][pre.deckCount[nextPlayer] - 1];
    //Remove card from deck
    pre.deck[nextPlayer][pre.deckCount[nextPlayer] - 1] = -1;
    //Decrement deck count
    pre.deckCount[nextPlayer]--;

    //Compare revealed cards
    if(tributeRevealedCards[0] == tributeRevealedCards[1])
    {
      tributeRevealedCards[1] = -1;
    }

    //Give bonus based on revealed card
    for(i = 0; i < 2; i++)
    {
      if (tributeRevealedCards[i] == copper || tributeRevealedCards[i] == silver || tributeRevealedCards[i] == gold)
      {
        pre.coins += 2;
      }

      //Victory Card Found, draw 2 cards
      else if (tributeRevealedCards[i] == estate || tributeRevealedCards[i] == duchy || tributeRevealedCards[i] == province || tributeRevealedCards[i] == gardens || tributeRevealedCards[i] == great_hall)
      {
        pre.handCount[currPlayer] += 2;
        if(pre.deckCount[currPlayer] < 2)
        {
          pre.deckCount[currPlayer] += pre.discardCount[currPlayer];
          pre.discardCount[currPlayer] = 0;
        }

        pre.deckCount[currPlayer] += 2;
      }

      //Action Card
      else
      {
        pre.numActions += 2;
      }
    }


    //Compare states
    printf("Coins are expected value\n");
    assertTrue(pre.coins == post->coins);

    printf("Actions are expected value\n");
    assertTrue(pre.numActions == post->numActions);

  }


}


int main()
{
  //Iterators for for loops
  int i, j;

  //Array of kingdom cards for game
  int kingdomCards[10] = {adventurer, council_room, feast, gardens, mine,
      remodel, smithy, tribute, baron, great_hall};


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
    printf("Players: %d\n\n", TestGame.numPlayers);

    //Set current player to random player
    int currPlayer = floor(Random() * TestGame.numPlayers);
    printf("Current Player: %d\n\n", currPlayer);
    TestGame.whoseTurn = currPlayer;

    //Set number of Actions = 1 as each turn should start this way
    TestGame.numActions = 1;


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


      int nextPlayer = currPlayer + 1;


      //Randomize hand and discard count for next player
      if(nextPlayer >= TestGame.numPlayers)
      {
        nextPlayer = 0;
      }

      printf("Next player is player %d\n", nextPlayer + 1);


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


    checkTribute(&TestGame, nextPlayer, currPlayer);

    testCounter++;
  }


  return 0;
}
