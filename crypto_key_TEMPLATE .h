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
	0x01, 0x23, 0x45, 0x67, 0x89, 0x9A, 0xAB, 0xCD  \
}

#ifdef __cplusplus
}
#endif

#endif //_crypto_key_H_
/*** EOF ***/
