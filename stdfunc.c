/****************************************************************************
 *
 *  Description: A C implementation of the Serpent encryption algorythmus
 *
 *  Author: WE ARE ANONYMOUS; EXPECT US. 
 *
 *  Copyrights. None. This is purely public domain
 *
 *  File: stdfunc.c
 *
 *  Description: standard function replacements
 *
 *  Version 1.0
 *
 *  Project: Paranoid Serpent encryption tool
 *
 ****************************************************************************/

/****************************************************************************
*                              INCLUDES
*****************************************************************************/
#include "platform_types.h"
#include "stdfunc.h"
#include <stdio.h>
#include <unistd.h>
 #include <termios.h>
/****************************************************************************
*                              DEFINES
*****************************************************************************/

/****************************************************************************
*                              MACROS
*****************************************************************************/

/****************************************************************************
*                              LOCAL VARIABLES
*****************************************************************************/

/****************************************************************************
*                              GLOBAL VARIABLES
*****************************************************************************/

/****************************************************************************
*                              LOCAL FUNCTIONS
*****************************************************************************/

/****************************************************************************
*                             GLOBAL FUNCTIONS
*****************************************************************************/

/****************************************************************************
* FUNC:		my_memclr
* PARAM:	u8* pData, u32 size
* RET:		void
* DESC:		replacement of memset(x, 0, y)
*****************************************************************************/
void my_memclr(u8* pData, u32 size) 
{
	for(int i = 0; i < size; i++) {
		pData[i] = 0;
	}
}

/****************************************************************************
* FUNC:		my_strlen
* PARAM:	const char *s
* RET:		u32
* DESC:		replacement of strlen()
*****************************************************************************/
u32 my_strlen(const char *s)
{
	/***** DEFINITION *******/
	u32 result;
	/**** INITIALIZATION ****/
	result = 0;
	/**** PARAMETER CHECK ***/
	if(s == NULL) {
		return 0;
	}
	/**** PROGRAM CODE ******/
	
	while(result < 0xFFFFFFFF && s[result] != 0) {
		result++;
	}
	return result;
}

/****************************************************************************
* FUNC:		my_strcpy
* PARAM:	const char *s
* RET:		u32
* DESC:		replacement of strlen()
*****************************************************************************/
bool my_strcpy(char *pOut, const char *pIn, u32 maxlen)
{
	/***** DEFINITION *******/
	u32 i;
	/**** PARAMETER CHECK ***/
	if(pOut == NULL || pIn == NULL) {
		return false;
	}
	if(maxlen == 0) {
		return false;
	}
	/**** INITIALIZATION ****/
	/**** PROGRAM CODE ******/
	
	for(i = 0; i < maxlen-1; i++) {
		*pOut = *pIn;
		pIn++;
		pOut++;
		if(*pIn == 0) { /* test for end of string */
			*pOut = 0;
			return true;
		}
	}
	return false;
}

/****************************************************************************
* FUNC:		scrambleByPwd
* PARAM:	u8* pInput, u32 maxlen
* RET:		void
* DESC:		scramble a key by user character input
*****************************************************************************/
int scrambleByPwd(u8* pInput, u32 maxlen)
{
	/***** DEFINITION *******/
	char c;
	u32 i;
	/**** PARAMETER CHECK ***/
	/**** INITIALIZATION ****/
	/**** PROGRAM CODE ******/
	
	for(i = 0; i < maxlen; i++) {
		c = getc( stdin );
		if(c == '\n' || c == '\r') {
			return i;
		}
		pInput[i] ^= c;
	}
	return i;
}

/****************************************************************************
* FUNC:		isValidString
* PARAM:	char *pInput
* RET:		void
* DESC:		returns true if the provided string is a valid ASCII string
*****************************************************************************/
bool isValidString(char *pInput)
{
	/***** DEFINITION *******/
	u8 c;
	u32 i;
	/**** PARAMETER CHECK ***/
	/**** INITIALIZATION ****/
	c = 'a';
	/**** PROGRAM CODE ******/
	
	for(i = 0; i < 0xFFFFFFFF && c != 0; i++) {
		c = pInput[i];
		switch(c) {
			case '<':
			case '>':
			case '|':
			case ':':
			case '?':
			case '*':
			case '\\':
			case '/':
			case '"':

			return false;
			break;
		}
		if(c > 0 && c < 32) {
			return false;
		}
	}
	return true;
}

/*** EOF ***/


