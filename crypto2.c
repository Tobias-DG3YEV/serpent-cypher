/****************************************************************************
 *
 *  Description: A C implementation of the Serpent encryption algorythmus
 *
 *  Author: WE ARE ANONYMOUS; EXPECT US. 
 *
 *  Copyrights. None. This is purely public domain
 *
 *  File: crypto2.c
 *
 *  Description: Serpent cypher routines
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
#include <string.h>
#include "crypto_sbox.h"
#include "crypto2.h"

#ifdef CRYPTO_PLACE_INTO_TCM /* place into tigh coupled memory (e.g. embedded systems)*/
#pragma section = "ITCM"
#endif

/****************************************************************************
*                              DEFINES
*****************************************************************************/

//#define CRYPTO_PASSTHROUGH /* deactivates de/encryption for stream testing purposes */

#ifndef CRYPTO2_CONV_KEY /* use convolutional encryption default on */
/* This means next block en/decryption depends on content of the preceding block */
#define CRYPTO2_CONV_KEY	1
#endif /* CRYPTO2_CONV_KEY */

typedef struct S_CRYPTOBLOCK {
    u32 part[4];
} T_CRYPTOBLOCK;

/****************************************************************************
*                              MACROS
*****************************************************************************/

#define ROR(x, n)  ( ((x) >> (n)) | ((x) << (32-(n))) ) /* rotate 32 bits right by n */
#define ROL(x, n)  ( ((x) << (n)) | ((x) >> (32-(n))) ) /* rotate 32 bits left by n  */

#define GETDATABLOCK(x)		(d[(x)*4+0] | d[(x)*4+1]<<8 | d[(x)*4+2]<<16 | d[(x)*4+3]<<24)
#define STOREDATABLOCK(x, y) d[(x)*4+0] = (y) & 0xFF; \
                             d[(x)*4+1] = (y>>8) & 0xFF; \
                             d[(x)*4+2] = (y>>16) & 0xFF; \
                             d[(x)*4+3] = (y>>24) & 0xFF;


/****************************************************************************
*                              LOCAL VARIABLES
*****************************************************************************/

/********************/
/*  Subsitution Box */
/********************/

#ifdef _MSC_BUILD  /* is it compiled for Windows? */
const u32 sboxtab[16] = CRYPTO_SBOX_VALUES;
#elifdef __linux__
const u32 sboxtab[16] = CRYPTO_SBOX_VALUES;
#else /* embedded systems */
const u32 sboxtab[16] @ "DTCM" = CRYPTO_SBOX_VALUES;
#endif

/****************************************************************************
*                              GLOBAL VARIABLES
*****************************************************************************/

/****************************************************************************
*                              LOCAL FUNCTIONS
*****************************************************************************/

/****************************************************************************
* FUNC:		sbox
* PARAM:	u32 orig, UTINY boxNr
* RET:		u32
* DESC:		Substitudes a nibble from the given dword and returns manipulated
*			    dword. Only 4 bits in a dword are substituted
*****************************************************************************/
static u32 sbox(u32 orig, UTINY boxNr)
{
	/***** DEFINITION *******/
	UTINY ornib; /* nibble of the original input value */
	u32 newnib; /* new nibble, alread in the new position within dword */
	u32 result; /* new dword */

	/**** INITIALIZATION ****/
	boxNr = boxNr % 8; /* a maximum of 8 SBoxes exist, numbered from 0 t 7 */

	/**** PARAMETER CHECK ***/
	/* any parameter combination is allowed */

	/**** PROGRAM CODE ******/
	ornib = (orig >> CRYPTO2_SBOX_SHIFT) & 0x0F; /* only 4 bits for a substitution */
	newnib = sboxtab[ornib] & (0xF << (boxNr*4));
	newnib = (newnib >> (boxNr*4)) << CRYPTO2_SBOX_SHIFT;
	result = orig & ~((u32)0xF << CRYPTO2_SBOX_SHIFT); /* clear out the sub-nibble position */
	result |= newnib; /* shift nibble back to the dword position and map it*/

	return result;
}

/****************************************************************************
* FUNC:		transform_to
* PARAM:	T_CRYPTOBLOCK  *db
* RET:		void
* DESC:		transforms a data block into encrypted block
*****************************************************************************/
static void transform_to(T_CRYPTOBLOCK *db)
{
	/***** DEFINITION *******/
	u32 a, b, c, d;

	/**** INITIALIZATION ****/
	/**** PARAMETER CHECK ***/
	if(db == NULL) {
		return;
	}
	/**** PROGRAM CODE ******/

	a = db->part[0];
	b = db->part[1];
	c = db->part[2];
	d = db->part[3];

	a = ROL(a, 13);
	c = ROL(c, 3);
	b = a ^ b ^ c;
	d = d ^ c ^ ROL(a,3);
	b = ROL(b,1);
	d = ROL(d,7);
	a = a ^ b ^ d;
	c = ROL(b, 7) ^ c ^ d;
	a = ROL(a, 5);
	c = ROL(c, 22);

	db->part[0] = a;
	db->part[1] = b;
	db->part[2] = c;
	db->part[3] = d;
}

/****************************************************************************
* FUNC:		transform_from
* PARAM:	T_CRYPTOBLOCK  *db
* RET:		void
* DESC:		transforms an encrypted block into original data
*****************************************************************************/
static void transform_from(T_CRYPTOBLOCK *db)
{
	/***** DEFINITION *******/
	u32 a, b, c, d;

	/**** INITIALIZATION ****/
	/**** PARAMETER CHECK ***/
	if(db == NULL) {
		return;
	}
	/**** PROGRAM CODE ******/

	a = db->part[0];
	b = db->part[1];
	c = db->part[2];
	d = db->part[3];

	c = ROR(c, 22);
	a = ROR(a, 5);
	c = ROL(b, 7) ^ c ^ d;
	a = a ^ b ^ d;
	d = ROR(d,7);
	b = ROR(b,1);
	d = d ^ c ^  ROL(a,3);
	b = a ^ b ^ c;
	a = ROR(a, 13);
	c = ROR(c, 3);

	db->part[0] = a;
	db->part[1] = b;
	db->part[2] = c;
	db->part[3] = d;
}

/****************************************************************************
* FUNC:		build_key
* PARAM:	T_CRYPTOBUF *pRing
* RET:		void
* DESC:		build the 32x128bit crypto key from original 32 byte key
*****************************************************************************/
static void build_key(T_CRYPTOBUF *pRing)
{
	/***** DEFINITION *******/
    u32 i, j;
	/**** INITIALIZATION ****/
    u32 w = 0;
	/**** PARAMETER CHECK ***/
	/**** PROGRAM CODE ******/
    /* key must be a 32 byte array */
    /* create the 32 indiviual 128 Bit keys into buffer */
    for(i = 0; i < CRYPTO2_KEY_SIZE *4; i++) {
        j = i / 4; /* j is from 0 to CRYPTO2_KEY_SIZE-1 */
        pRing->key[j].part[i%4] = pRing->origkey[i%CRYPTO2_KEY_SIZE]
                                + w
                                + ((pRing->origkey[31-(i%32)])<<8);

        w += ((pRing->origkey[i%32])<<16) + ((pRing->origkey[31-(i%32)])<<24);
    }
}


/****************************************************************************
* FUNC:		enc
* PARAM:	u8* d, T_CRYPTOBUF *pRing
* RET:		static void
* DESC:		internal block encryption routine
*****************************************************************************/
static void enc(u8* d, T_CRYPTOBUF *pRing)
{
	/***** DEFINITION *******/
    u32 i, j;
    u32 w;
	T_CRYPTOBLOCK db;
	/**** INITIALIZATION ****/
	/**** PARAMETER CHECK ***/
	/**** PROGRAM CODE ******/

	/* load from stream into internal datablock */
	db.part[0] = GETDATABLOCK(0);
	db.part[1] = GETDATABLOCK(1);
	db.part[2] = GETDATABLOCK(2);
	db.part[3] = GETDATABLOCK(3);

	for(i = 0; i <= 31; i++) { /* crypto rounds */
      /* bitwise XOR with Ki and substitution */
      for(j = 0; j <= 3; j++) {
			/* XOR with key */
      w = db.part[j] ^ pRing->key[i].part[j];
			/* substitute the same 4 bits in each data block */
      w = sbox(w, i);
      db.part[j] = w;
		}
		/* transformation */
		transform_to(&db);
		/* next round */
  }

	/* build new key */
	/* key regeneration */
	for(i = 0; i < 4; i++) {
		pRing->origkey[i*8] ^= db.part[i];
		pRing->origkey[i*8+1] ^= db.part[i]>>7;
		pRing->origkey[i*8+2] ^= db.part[i]>>12;
		pRing->origkey[i*8+3] ^= db.part[i]>>18;
		pRing->origkey[i*8+4] ^= db.part[i]>>3;
		pRing->origkey[i*8+5] ^= db.part[i]>>9;
		pRing->origkey[i*8+6] ^= db.part[i]>>13;
		pRing->origkey[i*8+7] ^= db.part[i]>>23;
	}
	build_key(pRing);
	/* store back into stream */
	STOREDATABLOCK(0, db.part[0]);
	STOREDATABLOCK(1, db.part[1]);
	STOREDATABLOCK(2, db.part[2]);
	STOREDATABLOCK(3, db.part[3]);
}

/****************************************************************************
* FUNC:		dec
* PARAM:	u8* d, T_CRYPTOBUF *pRing
* RET:		static void
* DESC:		internal block encryption routine
*****************************************************************************/
static void dec(u8* d, T_CRYPTOBUF *pRing)
{
	/***** DEFINITION *******/
    int i, j;
    u32 w;
	T_CRYPTOBLOCK db;
	/**** INITIALIZATION ****/
	/**** PARAMETER CHECK ***/
	/**** PROGRAM CODE ******/

	db.part[0] = GETDATABLOCK(0);
	db.part[1] = GETDATABLOCK(1);
	db.part[2] = GETDATABLOCK(2);
	db.part[3] = GETDATABLOCK(3);

	/* key regeneration */
	for(i = 0; i < 4; i++) {
		pRing->origkey[i*8] ^= db.part[i];
		pRing->origkey[i*8+1] ^= db.part[i]>>7;
		pRing->origkey[i*8+2] ^= db.part[i]>>12;
		pRing->origkey[i*8+3] ^= db.part[i]>>18;
		pRing->origkey[i*8+4] ^= db.part[i]>>3;
		pRing->origkey[i*8+5] ^= db.part[i]>>9;
		pRing->origkey[i*8+6] ^= db.part[i]>>13;
		pRing->origkey[i*8+7] ^= db.part[i]>>23;
	}

    for(i = 31; i >= 0; i--) { /**** crypto rounds ****/
		/* transformation */
		transform_from(&db);

		for(j = 0; j <= 3; j++) {
			/* substitute the same 4 bits in each data block */
            w = db.part[j];
			/* XOR with key */
            w = pRing->key[i].part[j] ^ sbox(w, i);
            db.part[j] = w;
        }
		/* next round */
    };

	/* build new key */
	build_key(pRing);

	STOREDATABLOCK(0, db.part[0]);
	STOREDATABLOCK(1, db.part[1]);
	STOREDATABLOCK(2, db.part[2]);
	STOREDATABLOCK(3, db.part[3]);

}

/****************************************************************************
*                             GLOBAL FUNCTIONS
*****************************************************************************/

/****************************************************************************
* FUNC:		CRYPTO2_init
* PARAM:	u8 *key, T_CRYPTOBUF *pRing
* RET:		void
* DESC:		Initialises the crypto engine
*****************************************************************************/
void CRYPTO2_init(const u8 *key, T_CRYPTOBUF *pRing)
{
	/***** DEFINITION *******/
	/**** INITIALIZATION ****/
	/**** PARAMETER CHECK ***/
	if(pRing == NULL) {
		return;
	}
	/**** PROGRAM CODE ******/
	memcpy(pRing->origkey, key, CRYPTO2_KEY_SIZE);
	build_key(pRing);
}

/****************************************************************************
* FUNC:		CRYPTO2_encrypt
* PARAM:	u8* pData, u32 size, T_CRYPTOBUF *pRing
* RET:		void
* DESC:		encrypts a given buffer (size multiple of 16)
*****************************************************************************/
void CRYPTO2_encrypt(u8* pData, u32 size, T_CRYPTOBUF *pRing)
{
	/***** DEFINITION *******/
    u32 i;
	/**** INITIALIZATION ****/
	/**** PARAMETER CHECK ***/
	if(pRing == NULL) {
		return;
	}
	if(pData == NULL) {
		return;
	}
	if(size % CRYPTO_BLOCK_LEN) {
		return;
	}
	/**** PROGRAM CODE ******/
#ifndef PASSTHROUGH
  for(i = 0; i < size; i += CRYPTO_BLOCK_LEN) {
    /* prepare data into 32 bit blocks */
    enc(pData+i, pRing);
    /* d is ready as 4 x 32 data bits */
  }
#endif
}

/****************************************************************************
* FUNC:		CRYPTO2_decrypt
* PARAM:	u8* pData, u32 size, T_CRYPTOBUF *pRing
* RET:		void
* DESC:		encrypts a given buffer (size multiple of 16)
*****************************************************************************/
void CRYPTO2_decrypt(void* pEncData, u32 size, T_CRYPTOBUF *pRing)
{
	/***** DEFINITION *******/
  u32 i;
	u8 *pData;
	/**** INITIALIZATION ****/
	pData = (u8*)pEncData;
	/**** PARAMETER CHECK ***/
	if(pRing == NULL) {
		return;
	}
	if(pData == NULL) {
		return;
	}
	if(size % CRYPTO_BLOCK_LEN) {
		return;
	}
	/**** PROGRAM CODE ******/
#ifndef PASSTHROUGH
  for(i = 0; i < size; i += CRYPTO_BLOCK_LEN) {
    /* prepare data into 32 bit blocks */
    dec(pData+i, pRing);
    /* d is ready as 4 x 32 data bits */
  }
#endif
}

#ifdef CRYPTO_PLACE_INTO_ITCM
#pragma section = ""
#endif

/*** EOF ***/
