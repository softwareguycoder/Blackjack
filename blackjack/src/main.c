#include "stdafx.h"
#include "symbols.h"

#include "blackjack.h"
#include "card.h"
#include "player.h"
#include "utilities.h"

#define  PLAYER1_WON    "*** PLAYER #1 WON!! ***\n"
#define  DEALER_WON     "*** COMPUTER WON!!  Better luck next time! ***\n"

PPLAYER pPlayer1;
PPLAYER pDealer;

// Global that indicates whether the user is still hitting
BOOL fIsHitting;

// Global that indicates whether the user still wants to play
BOOL fIsPlaying;

void FlushStandardInput()
{
    fseek(stdin,0,SEEK_END);
}

void FindOutWhoWon()
{
    /* Double-check that a player is initialized. */
    if (!IsPlayerInitialized(pPlayer1)
        || !IsPlayerInitialized(pDealer))
    {
        error("Unable to tell who won because a player is not initialized.\n");        
    }
    
    if (IsPlayerBust(pPlayer1))
    {
        /* If player 1 went bust then the computer automatically wins */
        fprintf(stdout, DEALER_WON);
        return;
    }

    /* Turn all the Dealer's cards Face Up, make the dealer 'hit'
     * until the Dealer has at least a score of 17, and then print
     * the game's state and decide who is the winner */
    TurnUpAllCards(pDealer);
    
    /* Make the dealer 'hit' until the dealer has at least 17 points */
    while(GetPlayerScore(pDealer) < DEALER_HIT_LIMIT)
    {
        DrawNewCard(pDealer);
    }
        
    PrintGameState();
    
    /* If the dealer has gone bust, then the Player automatically wins */
    if (IsPlayerBust(pDealer))
    {
        fprintf(stdout, PLAYER1_WON);
        return;
    }
    
    if (PlayerHasBlackjack(pPlayer1))
    {
        fprintf(stdout, PLAYER1_WON);
        return;
    }
    
    if (PlayerHasBlackjack(pDealer))
    {
        fprintf(stdout, DEALER_WON);
        return;
    }
    
    /* If we have flowed down to this level, simply do a 
     * score comparison between player and dealer */
    int player1Score = GetPlayerScore(pPlayer1);
    int dealerScore = GetPlayerScore(pDealer);
    
    if (player1Score == dealerScore)
    {
        fprintf(stdout, "*** PUSH ***\n");
        return;
    }
    else if (player1Score > dealerScore)
    {
        fprintf(stdout, PLAYER1_WON);
        return;
    }
    else if (player1Score < dealerScore)
    {
        fprintf(stdout, DEALER_WON);
        return;
    }
}

BOOL ShouldStartNewGame()
{
    char szAnswer[3];
    fprintf(stdout, "\n> Do you want to start over (N = quit)? (Y/N)[Y] > ");
    fflush(stdout);
    if (fgets(szAnswer, 3, stdin) == NULL)
    {
        strcpy(szAnswer, "Y");
    }
    FlushStandardInput();

    // strip trailing newline from fgets result
    char *pos;
    if (szAnswer == NULL || szAnswer[0] == '\n' || szAnswer[0] == '\0')
        strcpy(szAnswer, "Y");
    else if ((pos=strchr(szAnswer, '\n')) != NULL)
        *pos = '\0';
    
    return (strlen(szAnswer) > 0) 
        && (szAnswer[0] == 'Y' || szAnswer[0] == 'y');
}

BOOL ShouldContinue()
{
    char szAnswer[3];
    fprintf(stdout, "\n> Do you want another card? (Y/N)[Y] > ");
    fflush(stdout);
    if (fgets(szAnswer, 3, stdin) == NULL)
    {
        strcpy(szAnswer, "Y");
    }
    FlushStandardInput();

    // strip trailing newline from fgets result
    char *pos;
    if (szAnswer == NULL || szAnswer[0] == '\n' || szAnswer[0] == '\0')
        strcpy(szAnswer, "Y");
    else if ((pos=strchr(szAnswer, '\n')) != NULL)
        *pos = '\0';
    
    return (strlen(szAnswer) > 0) 
        && (szAnswer[0] == 'Y' || szAnswer[0] == 'y');
}

void PrintGameState()
{
    if (!IsPlayerInitialized(pPlayer1)
        || !IsPlayerInitialized(pDealer))
    {
        error("Must have a player and a dealer initialized to print state.\n");
    }

    PrintTitle();
    
    PrintPlayer(pPlayer1);
    PrintPlayer(pDealer);
}

void PrintTitle()
{
    clear();
    fprintf(stdout, "Welcome to the Blackjack game!\n");
    fprintf(stdout, "****Where 'someone' might be a WINNER!!!****\n\n");
}

void FreeObject(void** ppObj)
{
    if (ppObj == NULL || *ppObj == NULL)
        return;
        
    free(*ppObj);
    *ppObj = NULL;
}

int main (int argc, char* argv[])
{
    PrintTitle();
    
    fIsPlaying = TRUE;
    fIsHitting = TRUE;
    
    time_t t;
    
    /* Intializes random number generator */
    srand((unsigned) time(&t));
    
    fprintf(stdout, "> What is your name? (%d chars max) > ", USERNAME_MAX);
    
    char szUserName[USERNAME_MAX + 1];
    if (fgets(szUserName, USERNAME_MAX + 1, stdin)== NULL)
    {
        error("You must provide a name for your player!\n");
    }
    FlushStandardInput();
    
    // Remove trailing newline from szUserName
    char *pos;
    if ((pos=strchr(szUserName, '\n')) != NULL)
        *pos = '\0';
    else
        error("Player name: input too long for buffer.\n");

    while (fIsPlaying)
    {
        pPlayer1 = CreatePlayer(szUserName, FALSE);
        pDealer = CreatePlayer("Computer", TRUE);
            
        while(fIsHitting)
        {
            PrintGameState();
            
            if (IsPlayerBust(pPlayer1))
                break;
                
            fIsHitting = ShouldContinue();
            if (fIsHitting)
                DrawNewCard(pPlayer1);
            else
                break;
        }
        
        FindOutWhoWon();
        
        FreePlayer(&pPlayer1);
        FreePlayer(&pDealer);
        
        fprintf(stdout, "\n*** GAME OVER!!! THANKS FOR PLAYING! ***\n");
        
        if (!ShouldStartNewGame())
        {
            fIsPlaying = FALSE;
            break;
        }
        
        fIsHitting = TRUE;
    }
    
    return 0;
}