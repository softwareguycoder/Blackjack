#include "stdafx.h"
#include "symbols.h"

#include "blackjack.h"
#include "card.h"
#include "player.h"

#define INITIAL_CARD_COUNT          2       // Initial count of cards in a hand

void TurnUpAllCards(PPLAYER pPlayer)
{
    if (!IsPlayerInitialized(pPlayer))
    {
        error("Player object is not properly initialized.\n");
    }
    
    // Set the bFaceDown member of all the player's cards to FALSE
    for(int i = 0;i < pPlayer->nCardCount;i++)
    {
        pPlayer->pHand[i].bFaceDown = FALSE;
    }
}

BOOL PlayerHasFaceCard(PPLAYER pPlayer)
{
    BOOL bResult = FALSE;
    if (!IsPlayerInitialized(pPlayer))
    {
        error("Player object is not properly initialized.\n");
    }
 
    for(int i = 0;i < pPlayer->nCardCount;i++)
    {
        bResult = pPlayer->pHand[i].nValue >= JACK 
            && pPlayer->pHand[i].nValue < ACEHI;
        if (bResult)
            break;
    }

    return bResult;    
}

BOOL PlayerHasBlackjack(PPLAYER pPlayer)
{
    if (!IsPlayerInitialized(pPlayer))
    {
        error("Player object is not properly initialized.\n");
    }
    
    /* Since a "blackjack hand" is defined by the rules to contain exactly
     * two cards, if this is not the case, we know right off the bat, that
     * player does not have a blackjack. */
    if (pPlayer->nCardCount != 2)
        return FALSE;
    
    return PlayerHasAce(pPlayer) && PlayerHasFaceCard(pPlayer);
}

BOOL PlayerHasAce(PPLAYER pPlayer)
{
    BOOL bResult = FALSE;
    if (!IsPlayerInitialized(pPlayer))
    {
        error("Player object is not properly initialized.\n");
    }
    
    for(int i = 0;i < pPlayer->nCardCount;i++)
    {
        bResult = pPlayer->pHand[i].nValue == ACELO
            || pPlayer->pHand[i].nValue == ACEHI;
        if (bResult)
            break;
    }
    
    return bResult;
}

BOOL IsPlayerBust(PPLAYER pPlayer)
{
    /* A player is "bust" if their hand totals more than 21 points */
    if (!IsPlayerInitialized(pPlayer))
    {
        error("IsPlayerBust: Player object is not properly initialized.\n");
    }    
    
    return GetPlayerScore(pPlayer) > BLACKJACK_WINNING_SCORE;
}

void DrawNewCard(PPLAYER pPlayer)
{
    if (!IsPlayerInitialized(pPlayer))
    {
        error("Player cannot draw a new card.  Player is not properly initialized.\n");       
    }
    
    pPlayer->pHand = (PCARD)realloc((void*)(pPlayer->pHand), (pPlayer->nCardCount + 1)*sizeof(CARD));
    if (pPlayer->pHand == NULL)
    {
        error("There is insufficient memory for the operation.\n");
    }
    
    pPlayer->nCardCount += 1;
    
    pPlayer->pHand[pPlayer->nCardCount - 1].nValue = rand() % 13;
    pPlayer->pHand[pPlayer->nCardCount - 1].nSuit = rand() % 4;
    
    // Whenever the player or the dealer hits, the new
    // card is always face up
    pPlayer->pHand[pPlayer->nCardCount - 1].bFaceDown = FALSE;
}

int GetPlayerScore(PPLAYER pPlayer)
{
    if (!IsPlayerInitialized(pPlayer))
    {
        error("GetPlayerScore: The player is not properly initialized.\n");
    }
    
    int nResult = 0;
    for(int i = 0;i < pPlayer->nCardCount;i++)
    {
        // Skip face-down cards 
        if (pPlayer->pHand[i].bFaceDown)
            continue;
            
        // Add the points that the player gets for having the current
        // card to the total score that is in nResult.  The ScoreOf
        // function, implemented in utilities.c, is responsible for assigning
        // the points for a given card.
        nResult += ScoreOf(pPlayer->pHand[i].nValue);
    }
    
    return nResult;
}

void FreePlayer(PPLAYER *ppPlayer)
{
    if (ppPlayer == NULL || *ppPlayer == NULL)
        return;
        
    FreeObject((void**)&((*ppPlayer)->pHand));
    FreeObject((void**)ppPlayer); 
}

BOOL IsPlayerInitialized(PPLAYER pPlayer)
{
    /* Given the pointer, pPlayer, to a PLAYER structure provided in the 
     * argument to this function, check that it is not NULL, and that the
     * szUserName parameter is filled in, and that the player  has at least
     * two cards in his/her hand. */
    
    /* If pPlayer is NULL, the player is not properly initialized. */
    if (pPlayer == NULL) 
    {
        return FALSE;    
    }
    
    /* If the user name of the player is blank, then the player is not
     * properly initialized. */
    if (pPlayer->szUserName == NULL
        || strlen(pPlayer->szUserName) == 0
        || pPlayer->szUserName[0] == '\0')
    {
        return FALSE;
    }
    
    /* If the player has less than 2 cards in his/her hand, then the 
     * player is not properly initialized. */
    if (pPlayer->nCardCount < 2) 
    {
        return FALSE;
    }
    
    /* If we are here, then the player passed to us meets all the 
     * criteria for being properly initialized. */
    
    return TRUE;
}

void PrintPlayer(PPLAYER pPlayer)
{
    if (!IsPlayerInitialized(pPlayer))
    {
        error("PrintPlayer: Player structure not initialized.\n");
    }
    
    fprintf(stdout, "Player: %s\n", pPlayer->szUserName);
    for(int i = 0;i < pPlayer->nCardCount;i++) 
    {
        if (pPlayer->pHand[i].bFaceDown == TRUE)
        {
            fprintf(stdout, "Card #%d: (face down)\n", i+1);
            continue;
        }
 
       fprintf(stdout, "Card #%d: %s of %s\n", i+1,
        ValueToString(pPlayer->pHand[i].nValue), 
            SuitToString(pPlayer->pHand[i].nSuit));
    }
    
    fprintf(stdout, "%s has %d points (not counting face-down cards)\n",
        pPlayer->szUserName, GetPlayerScore(pPlayer));
        
    if (IsPlayerBust(pPlayer)) 
    {
        if (pPlayer->bIsDealer)
            fprintf(stdout, "*** DEALER: BUST ***\n");
        else
            fprintf(stdout, "*** PLAYER #1: BUST ***\n");
    }
}

// Creates a new player and gives them a card.
PPLAYER CreatePlayer (const char *pszUserName, BOOL bIsDealer)
{
    if (pszUserName == NULL
        || strlen(pszUserName) == 0
        || pszUserName[0] == '\0')
    {
        error("A name for the new player is required.\n");
    }
    
    // Allocate memory for a new instance of the PLAYER structure
    PPLAYER pNewPlayer = (PPLAYER)malloc(sizeof(PLAYER));
    if (pNewPlayer == NULL)
    {
        error("There is insufficient memory available for execution.\n");
    }
    
    // the new player will be 'dealt' two cards in just a
    // second, but first, initialize the nCardCount member of the 
    // PLAYER structure to have the value of zero so there is not garbage
    // there.
    pNewPlayer->nCardCount = 0; 
    strcpy(pNewPlayer->szUserName, pszUserName);
    pNewPlayer->bIsDealer = bIsDealer;
    
    /* The requirements state that each new player must be given a 
     * hand consisting of 2 cards. */
    pNewPlayer->pHand = (PCARD)malloc(INITIAL_CARD_COUNT * sizeof(CARD));
    if (pNewPlayer->pHand == NULL) 
    {
        FreeObject((void*)pNewPlayer);
        error("There is insufficient memory available for execution.\n");
    }
    pNewPlayer->nCardCount = INITIAL_CARD_COUNT;
    
    PCARD currentCard = pNewPlayer->pHand;
    
    /* We use rand() modulus 13 for the value of the card as opposed to 
     * 14, since we want to avoid "dealing a player the high ace."  We should
     * only deal with the Ace = 11 points case in calling code. */
    InitializeCard(currentCard, rand() % 13, rand() % 4, FALSE);
    InitializeCard(++currentCard, rand() % 13, rand() % 4, bIsDealer 
        /* Player's second card is face down if they are the dealer */);
    
    return pNewPlayer;
}
