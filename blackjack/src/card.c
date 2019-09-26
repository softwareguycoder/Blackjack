#include "stdafx.h"
#include "symbols.h"

#include "card.h"

#define VALUE_ERROR_MESSAGE "Invalid value for nValue parameter. Must be between 0-13 inclusive.\n"
#define SUIT_ERROR_MESSAGE "Invalid value for nSuit parameter. Must be between 0-3 inclusive.\n"

char* ValueToString(int nValue)
{
    if (nValue < ACELO || nValue > ACEHI) {
        error(VALUE_ERROR_MESSAGE);
    }
    
    char* result = NULL;
    switch(nValue) {
        case ACELO:
        case ACEHI:
            result = "Ace";
            break;
            
        case TWO:
            result = "Two";
            break;
            
        case THREE:
            result = "Three";
            break;

        case FOUR:
            result = "Four";
            break;

        case FIVE:
            result = "Five";
            break;

        case SIX:
            result = "Six";
            break;

        case SEVEN:
            result = "Seven";
            break;

        case EIGHT:
            result = "Eight";
            break;

        case NINE:
            result = "Nine";
            break;

        case TEN:
            result = "Ten";
            break;
            
        case JACK:
            result = "Jack";
            break;
            
        case QUEEN:
            result = "Queen";
            break;
            
        case KING:
            result = "King";
            break;

        default:
            result = "<unknown>";
            break;
    }
    return result;
}

char* SuitToString(int nSuit)
{
    if (nSuit < 0 || nSuit > 3) {
        error(SUIT_ERROR_MESSAGE);
    }
    
    char* result = NULL;
    switch(nSuit) {
        case SPADES:
            result = "Spades";
            break;
        
        case DIAMONDS:
            result = "Diamonds";
            break;
            
        case CLUBS:
            result = "Clubs";
            break;
            
        case HEARTS:
            result = "Hearts";
            break;
        
        default:
            result = "<unknown>";
            break;
    }
    return result;
}

void CardToString(PCARD pCard, char* buffer)
{
    if (pCard == NULL) {
        return;
    }
    
    ValidateValueAndSuit(pCard->nValue, pCard->nSuit);
    
    /* If we are here then the pCard has passed validation since 
     * the program has not been terminated by the ValidateValueAndSuit
     * function. */
    
    /* Formulate a string that corresponds to the card's value
     * and suit; such as 'Ace of Spades' or 'Two of Diamonds'
     * etc. */
    sprintf(buffer, "%s of %s", 
        ValueToString(pCard->nValue), SuitToString(pCard->nSuit));
}

void ValidateValueAndSuit(int nValue, int nSuit) 
{
    if (nValue < 0 || nValue > 13) {
        error(VALUE_ERROR_MESSAGE);
    }
    
    if (nSuit < 0 || nSuit > 3) {
        error(SUIT_ERROR_MESSAGE);
    }
}

void InitializeCard(PCARD pCard, int nValue, int nSuit, BOOL bFaceDown)
{
    if (pCard == NULL)
    {
        error("The InitializeCard function must be passed a non-NULL pointer value.\n");
    }
    
    /* this function forcibly quits the program with an error message
     * if the nValue or nSuit paramters do not have values within the bounds
     * of what is allowed for this program. */
    ValidateValueAndSuit(nValue, nSuit);
    
    pCard->nValue = nValue;
    pCard->nSuit = nSuit;
    pCard->bFaceDown = bFaceDown;
}

PCARD CreateNewCard(int nValue, int nSuit)
{
    /* this function forcibly quits the program with an error message
     * if the nValue or nSuit paramters do not have values within the bounds
     * of what is allowed for this program. */
    ValidateValueAndSuit(nValue, nSuit);
    
    PCARD pNewCard = (PCARD)malloc(sizeof(CARD));
    if (pNewCard == NULL) {
        error("There was a problem allocating the memory for a new card.\n");
    }
    
    pNewCard->nValue = nValue;
    pNewCard->nSuit = nSuit;
    pNewCard->bFaceDown = FALSE;

    return pNewCard;
}

BOOL IsFaceCard(PCARD pCard)
{
    if (pCard == NULL) {
        return FALSE;
    }
    
    return pCard->nValue == JACK || pCard->nValue == QUEEN || pCard->nValue == KING;
}
