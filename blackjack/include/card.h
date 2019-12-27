#ifndef __CARD_H__
#define __CARD_H__

#include "stdafx.h"
#include "symbols.h"

typedef struct _tagCARD {
    int     nValue;          /* value of the card */
    int     nSuit;           /* suit of the card */
    BOOL    bFaceDown;       /* Is this card face down? */
} CARD, *PCARD, **PPCARD;

void CardToString(PCARD pCard, char* buffer);
PCARD CreateNewCard(int nValue, int nSuit);
void InitializeCard(PCARD pCard, int nValue, int nSuit, BOOL bFaceDown);
BOOL IsFaceCard(PCARD pCard);
char* SuitToString(int nSuit);
void ValidateValueAndSuit(int nValue, int nSuit);
char* ValueToString(int nValue);

#endif  //__CARD_H__
