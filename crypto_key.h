/****************************************************************************
 *
 *  Description: A C implementation of the Serpent encryption algorythmus
 *
 *  Author: WE ARE ANONYMOUS; EXPECT US. 
 *
 *  Copyrights. None. This is purely public domain
 *
 *  File: crypto_key.h
 *
 *  Description: cypher key definition
 *
 *  Version 1.0
 *
 *  Project: Paranoid Serpent encryption tool
 *
 ****************************************************************************/
 
#ifndef _CRYPTO_KEY_H_
#define _CRYPTO_KEY_H_

#ifdef __cplusplus
extern "C" {
#endif

/****************************************************************************
*                              GLOBAL DEFINES
*****************************************************************************/

#define CRYPTO_KEY { \
	0x01, 0x23, 0x45, 0x67, 0x89, 0x9A, 0xAB, 0xCD, \
	0x01, 0x23, 0x45, 0x67, 0x89, 0x9A, 0xAB, 0xCD, \
	0x01, 0x23, 0x45, 0x67, 0x89, 0x9A, 0xAB, 0xCD, \
	0x01, 0x23, 0x45, 0x67, 0x89, 0x9A, 0xAB, 0xCD \
}

#ifdef __cplusplus
}
#endif

#endif //_CRYPTO_KEY_H_
/*** EOF ***/
