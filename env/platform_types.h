/****************************************************************************
 *
 *  Description: A C implementation of the Serpent encryption algorythmus
 *
 *  Author: WE ARE ANONYMOUS; EXPECT US. 
 *
 *  Copyrights. None. This is purely public domain
 *
 *  File: platform_types.h
 *
 *  Description: platform dependent definitions.
 *
 *  Version 1.0
 *
 *  Project: Paranoid Serpent encryption tool
 *
 ****************************************************************************/

#ifndef _PLATFORM_TYPES_H_
#define _PLATFORM_TYPES_H_

/****************************************************************************
*                              INCLUDES
*****************************************************************************/

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

/****************************************************************************
*                              DEFINES
*****************************************************************************/

/* our base types */
typedef unsigned char           u8;
typedef signed char             s8;
typedef unsigned short int      u16;
typedef signed short int        s16;
typedef unsigned int                            u32;
typedef signed long long                          s64;
typedef unsigned long long                      u64;
typedef signed int                              s32;

#define UTINY                   u32 /* this type can be a positive value from 0 to 255   */
#define STINY                   s32 /* this type can be any value from -127 to 128 */
#define USMALL          u32 /* this type can be a positive value from 0 to 65536 */
#define SSMALL          s32 /* this type can be any value from -32767 to 32768 */

typedef char                    t_string;

#define NULLPOINTER                                   ((void*)(0x00000000))

#ifndef NULL
  #define NULL   0
#endif /* NULL */

/* string type handling */
#ifdef _WIN32
#define STR(x) L##x
#define STR_T char
#else
#define STR(x) x
#define STR_T char
#endif

#if defined(__ICCARM__)
#define PACKED           __packed
#elif defined(__GNUC__)
#define PACKED           __attribute__((packed))
#endif

#define MIN(x,y)                                                                (((x)>(y))?(y):(x))
#define MAX(x,y)                            	                                    (((x)<(y))?(y):(x))
#define NUMOFELEM(x)                                            (sizeof((x))/sizeof((x)[0]))
#define SIZEOFELEM(x)                                           (sizeof((x)[0]))
#define HIGH16(x)                                                               (((x)>>8)&0xFF)
#define LOWBYTE(x)                                                      ((x)&0xFF)

#define HIGH_WORD(x)                                            (((x)>>16)&0xFFFF)
#define LOW_WORD(x)                                                     ((x)&0xFFFF)
#define HIGH_BYTE(x)                                            (((x)>>8)&0xFF)
#define LOW_BYTE(x)                                                     ((x)&0xFF)

#define GET_BYTE_0(x)           LOW_BYTE(x)
#define GET_BYTE_1(x)           HIGH_BYTE(x)
#define GET_BYTE_2(x)           LOW_BYTE(HIGH_WORD(x))
#define GET_BYTE_3(x)           HIGH_BYTE(HIGH_WORD(x))

#ifndef TRUE
# define TRUE 1
#endif

#ifndef FALSE
# define FALSE 0
#endif

#define ERROR(...) { printf("! ERROR: "); printf(__VA_ARGS__); printf ("\n"); return -1; }
#define LOG(...) { printf(__VA_ARGS__); printf ("\n"); }

#endif //_PLATFORM_TYPES_H_
/*** EOF ***/
