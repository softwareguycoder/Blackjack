#ifndef __UTILITIES_H__
#define __UTILITIES_H__

/**
 * \brief Writes the formatted message to stderr and then causes the 
 * program to immediately terminate with an exit code of ERROR.
 */
void error(const char* pszFormat, ...);

/**
 * \brief Transforms the numeric constant that gives what card this is (
 * an ACELO, TWO, or KING), specified in x, and returns the 'score',
 * or number of points, that the user gets for having this card; i.e., all 
 * face cards are 10 points, so ScoreOf(KING) == 10. */
int ScoreOf(int nValue);

#endif //__UTILITIES_H__