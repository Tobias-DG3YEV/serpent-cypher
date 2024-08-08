/****************************************************************************
 *
 *  Description: A C implementation of the Serpent encryption algorythmus
 *
 *  Author: WE ARE ANONYMOUS; EXPECT US. 
 *
 *  Copyrights. None. This is purely public domain
 *
 *  File: crypto2.h
 *
 *  Description: Serpent cypher routines
 *
 *  Version 1.0
 *
 *  Project: Paranoid Serpent encryption tool
 *
 ****************************************************************************/

#ifndef _CRYPTO2_H_
#define _CRYPTO2_H_

#ifdef _MSC_BUILD 
 #include "sys/types.h"
#endif
#ifdef __cplusplus
 extern "C" {
#endif
/****************************************************************************
*                              INCLUDES
*****************************************************************************/

/****************************************************************************
*                              GLOBAL DEFINES
*****************************************************************************/

#define CRYPTO2_KEY_SIZE			32
#define CRYPTO2_RING_SIZE        	32
#define CRYPTO_BLOCK_LEN         	16
#define CRYPTO2_CONV_KEY			1  /* 1 = use convoluting key */
#define CRYPTO2_SBOX_SHIFT			13 /* the standard postion for S-Box action */

typedef struct S_CRYPTOKEY {
    u32 part[4];
} T_CRYPTOKEY;

typedef struct S_CRYPTOBUF {
	u8 origkey[CRYPTO2_KEY_SIZE];
    T_CRYPTOKEY key[CRYPTO2_RING_SIZE];
} T_CRYPTOBUF;

/****************************************************************************
*                              MACROS
*****************************************************************************/

/****************************************************************************
*                              GLOBAL VARIABLES 
*****************************************************************************/

/****************************************************************************
*                            FUNCTION PROTOTYPES
*****************************************************************************/

/****************************************************************************
* Function Prototype: CRYPTO2_init
*****************************************************************************/
void CRYPTO2_init(const u8 *key, T_CRYPTOBUF *pRing);

/****************************************************************************
* Function Prototype: CRYPTO2_encrypt
*****************************************************************************/
void CRYPTO2_encrypt(u8* pData, u32 size, T_CRYPTOBUF *pRing);

/****************************************************************************
* Function Prototype: CRYPTO2_decrypt
*****************************************************************************/
void CRYPTO2_decrypt(void* pEncData, u32 size, T_CRYPTOBUF *pRing);

#ifdef __cplusplus
}
#endif

#endif //_CRYPTO2_H_
/*** EOF ***/
