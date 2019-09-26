////////////////////////////////////////////////////////////////////////////////
// Error-handling and other misc functions

#include "stdafx.h"
#include "blackjack.h"

#include "symbols.h"
#include "utilities.h"

int ScoreOf(int nValue) 
{
    if (nValue < ACELO || nValue > ACEHI) {
        error("Invalid card value\n");
    }
    
    if (nValue == ACEHI) {
        return HIGH_ACE_SCORE;      /* "high" Ace -- is 11 points */
    }
    
    int result = 0;
    if (nValue <= TEN){
        result = nValue + 1;
    } else {
    	result = FACE_CARD_SCORE;
    }

    return result;
}

void error(const char* pszFormat, ...)
{
    if (pszFormat == NULL
        || strlen(pszFormat) == 0
        || pszFormat[0] == '\0') {
            exit(-1);
            return;
    }
    
    va_list args;
    va_start(args, pszFormat);
    
    vfprintf(stderr, pszFormat, args);
    
    va_end(args);
    
    exit(ERROR);
}
