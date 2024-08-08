
/*HHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHH
 *
 *  Elektronik-idee Weber GmbH (c);
 *
 *  File: crypto_key.h
 *
 *  Version 1.0
 *
 *  Project: IoT-Prof
 *
 *  Description: 
 *
 *  tw@elektronik-idee.com
 *
 *  Copyright (C) 2020 - 2021 Elektronik-Idee, Tobias Weber
 *  All rights reserved, alle Rechte vorbehalten.
 *
 HHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHH*/
#ifndef _CRYPTO_SBOX_H_
#define _CRYPTO_SBOX_H_

/****************************************************************************
*                              GLOBAL DEFINES
*****************************************************************************/

/* each entry per column must be unique between 0x0 and 0xF */
#define CRYPTO_SBOX_VALUES { \
	0x6BF2AE17, \
	0xD2CE5403, \
	0x3180954F, \
	0x253CEB61, \
	0xF6A7D12B, \
	0x53B812FA, \
	0x047DB93C, \
	0xE964C8A0, \
	0xBC2587D9, \
	0xA7DF26C8, \
	0x9E4B0A75, \
	0x805A63E4, \
	0xC8137F96, \
	0x1F964D8E, \
	0x7AE130BD, \
	0x4D09FC52  \
}

#endif //_CRYPTO_SBOX_H_
/*** EOF ***/
