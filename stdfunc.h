/****************************************************************************
 *
 *  Description: A C implementation of the Serpent encryption algorythmus
 *
 *  Author: WE ARE ANONYMOUS; EXPECT US. 
 *
 *  Copyrights. None. This is purely public domain
 *
 *  File: stdfunc.h
 *
 *  Description:
 *
 *  Version 1.0
 *
 *  Project: Paranoid Serpent encryption tool
 *
 ****************************************************************************/

#ifndef _STDFUNC_H_
#define _STDFUNC_H_

/****************************************************************************
*                              INCLUDES
*****************************************************************************/
#include "platform_types.h"
#include <stdio.h>

/****************************************************************************
*                              GLOBAL DEFINES
*****************************************************************************/

#define MAX_FILENAMELEN		256 - 16 /* -8 for salt, -4 for size, -4 for checksum */

typedef struct {
	u8 salt[8];
	char fname[MAX_FILENAMELEN];
	u32 fsize;
	u32 crc;
} PACKED HEADER_T;

typedef struct {
	FILE * stream;
	char name[MAX_FILENAMELEN];
} FILE_T;

/****************************************************************************
*                              MACROS
*****************************************************************************/
/****************************************************************************
*                              GLOBAL VARIABLES
*****************************************************************************/
/****************************************************************************
*                            FUNCTION PROTOTYPES
*****************************************************************************/

void my_memclr(u8* pData, u32 size) ;
u32 my_strlen(const char *s);
bool my_strcpy(char *pOut, const char *pIn, u32 maxlen);
int scrambleByPwd(u8* pInput, u32 maxlen);
bool isValidString(char *pInput);

#endif //_STDFUNC_H_
/*** EOF ***/
