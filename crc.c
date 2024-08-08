/****************************************************************************
 *
 *  Description: A C implementation of the Serpent encryption algorythmus
 *
 *  Author: WE ARE ANONYMOUS; EXPECT US. 
 *
 *  Copyrights. None. This is purely public domain
 *
 *  File: crc.c
 *
 *  Description: cyclic redundancy checksum routines
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
* FUNC:		crc32
* PARAM:	void* pData, u32 size
* RET:		u32
* DESC:		calculates the CRC32 cehck sum of given data
*****************************************************************************/
u32 crc32(u32 crc, u8* pData, u32 len)
{
	/***** DEFINITION *******/
	static u32 table[256];
	static int have_table = 0;
	uint32_t rem;
	uint8_t octet;
	int i, j;
	const u8 *p, *q;
	/**** INITIALIZATION ****/
	/**** PARAMETER CHECK ***/
	if(pData == NULL) {
		ERROR("NULL pointer exception!");
	}
	/**** PROGRAM CODE ******/

	/* This check is not thread safe; there is no mutex. */
	if (have_table == 0) {
		/* Calculate CRC table. */
		for (i = 0; i < 256; i++) {
			rem = i;  /* remainder from polynomial division */
			for (j = 0; j < 8; j++) {
				if (rem & 1) {
					rem >>= 1;
					rem ^= 0xedb88320;
				} else
					rem >>= 1;
			}
			table[i] = rem;
		}
		have_table = 1;
	}

	crc = ~crc;
	q = &pData[len];
	for (p = pData; p < q; p++) {
		octet = *p;  /* Cast to unsigned octet. */
		crc = (crc >> 8) ^ table[(crc & 0xff) ^ octet];
	}
	return ~crc;
}


/*** EOF ***/
