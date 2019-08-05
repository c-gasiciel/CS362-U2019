#ifndef _DOMINION_HELPERS_H
#define _DOMINION_HELPERS_H

#include "dominion.h"

int drawCard(int player, struct gameState *state);
int updateCoins(int player, struct gameState *state, int bonus);
int discardCard(int handPos, int currentPlayer, struct gameState *state,
		int trashFlag);
int gainCard(int supplyPos, struct gameState *state, int toFlag, int player);
int getCost(int cardNumber);
int cardEffect(int card, int choice1, int choice2, int choice3,
	       struct gameState *state, int handPos, int *bonus);
int do_baron (struct gameState *state, int choice1, int currentPlayer);
int do_tribute(struct gameState *state, int tributeRevealedCards[2], int nextPlayer, int currentPlayer);
int do_ambassador(int j, int choice1, int choice2, int handPos, struct gameState *state, int currentPlayer);
int do_minion(int choice1, int choice2, int handPos, struct gameState *state, int currentPlayer);
int do_mine(int j, struct gameState *state, int choice1, int choice2, int currentPlayer, int handPos);
#endif
