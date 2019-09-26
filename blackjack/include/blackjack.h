#ifndef __BLACKJACK_H__
#define __BLACKJACK_H__

#define ERROR               -1          /* error exit code value */

#define clear() printf("\033[H\033[J")

void FreeObject(void** ppObj);
void PrintGameState();
void PrintTitle();

#endif //__BLACKJACK_H__