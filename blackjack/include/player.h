#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "stdafx.h"
#include "symbols.h"

/** \brief Defines a structure for a player.  We want the player's name, the
   cards that the player has in his or her hand, and a flag for whether
   the player is the dealer to be visible. */
typedef struct _tagPLAYER
{
  char      szUserName[USERNAME_MAX + 1];	/* username of the player */
  PCARD     pHand;		                    /* array of CARDs that constitutes 
                                                the player's hand */
  BOOL      bIsDealer;		                /* Is this player the dealer? */
  int       nCardCount;                     /* How many cards does player have? */
} PLAYER, *PPLAYER;

// Creates a new player and gives them a card.
PPLAYER CreatePlayer(const char *szUserName, BOOL bIsDealer);

// Puts a new card into the player's hand.
void DrawNewCard(PPLAYER pPlayer);

void FreePlayer(PPLAYER *ppPlayer);

// Gets a player's score.  Notice that the dealer's score
// must only be computed from face-up cards.
int GetPlayerScore(PPLAYER pPlayer);

/* Returns TRUE if the player has gone over the score of 21 */
BOOL IsPlayerBust(PPLAYER pPlayer);

/* Returns TRUE if the player specified is initialized (i.e., has their
 * username filled in and has at least 2 cards in their hand) or FALSE
 * otherwise. */
BOOL IsPlayerInitialized(PPLAYER pPlayer);

/* Returns TRUE if the player's hand contains an Ace */
BOOL PlayerHasAce(PPLAYER pPlayer);

/* Returns TRUE if the player has a Black Jack, which is
 * defined by the rules to be a hand that contains exactly 2 cards, which
 * consist of an Ace and a Face Card */
BOOL PlayerHasBlackjack(PPLAYER pPlayer);

/* Returns TRUE if the player's hand contains a Face Card */
BOOL PlayerHasFaceCard(PPLAYER pPlayer);

/* Prints out all the cards in the player's hand, and the player's name */
void PrintPlayer(PPLAYER pPlayer);

/* 'Turns up' all the cards of the player so that none of them are
 * face-down, i.e., sets the bFaceDown member to FALSE for all the player's
 * cards */
void TurnUpAllCards(PPLAYER pPlayer);

// TODO: Add declarations here

#endif //__PLAYER_H__
