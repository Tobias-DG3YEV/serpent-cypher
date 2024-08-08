/****************************************************************************
 *
 *  Description: A C implementation of the Serpent encryption algorythmus
 *
 *  Author: WE ARE ANONYMOUS; EXPECT US. 
 *
 *  Copyrights. None. This is purely public domain
 *
 *  File: SBoxGen.c
 *
 *  Description:
 *
 *  Version 1.0
 *
 *  Project: Paranoid Serpent encryption tool
 *
 ****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "platform_types.h"
#include "../crypto2.h"

/****************************************************************************
*                              DEFINES
*****************************************************************************/

#define SBOX_FILENAME	"crypto_sbox.h"

#define SBOX_ROWS 		CRYPTO_BLOCK_LEN
#define SBOX_COLS		8

/****************************************************************************
*                              LOCAL VARIABLES
*****************************************************************************/

u16 tmp[SBOX_ROWS][SBOX_COLS];

/****************************************************************************
*                              LOCAL FUNCTIONS
*****************************************************************************/

/****************************************************************************
* FUNC:		printSBox
* PARAM:	void
* RET:		void
* DESC:		
*****************************************************************************/
static void printSBox(void)
{
	/***** DEFINITION *******/
	int i, j;
	/**** PARAMETER CHECK ***/
	/**** INITIALIZATION ****/
	u16* pArray = (u16*)tmp;
	/**** PROGRAM CODE ******/

	printf("****** SBox *******\n");
	for(i = 0; i < SBOX_ROWS; i++) {
		for(j = 0;  j < SBOX_COLS; j++) {
		  printf(" %X", *pArray);
		  pArray++;
		}
		printf("\n");
	}
	printf("*******************\n");
}

/****************************************************************************
* FUNC:		createCol
* PARAM:	
* RET:		void
* DESC:		
*****************************************************************************/
static void createCol(int col)
{
	/***** DEFINITION *******/
	unsigned int row, p, r;
	/**** PARAMETER CHECK ***/
	/**** INITIALIZATION ****/
	/**** PROGRAM CODE ******/

	/* first mark all fields as free */
	for(row = 0; row < SBOX_ROWS; row++) {
		tmp[row][col] = 0x10 + row;
	}

  for(row = 0; row < SBOX_ROWS; row++) {
	  r = rand();
	  p = r % SBOX_ROWS;
	  //printf("%X ", p);
		
	if(tmp[row][col] >= 0x10) {
		/* check if other side is not already occupied */
		if(tmp[p][col] >=0x10) {
			tmp[row][col] = p;
			tmp[p][col] = row;
		}
		else {
			//printf("row %i\n", row);
			row--;
		}
	}
  }
}

/****************************************************************************
*                             GLOBAL FUNCTIONS
*****************************************************************************/

/****************************************************************************
* FUNC:		main
* PARAM:	int argc, char *argv[]
* RET:		int
* DESC:		program entry function
*****************************************************************************/
int main(int argc, char *argv[])
{
	/***** DEFINITION *******/
	int check[16];
	u16 x;
	int i, j, k, start;
	/**** PARAMETER CHECK ***/
	/**** INITIALIZATION ****/
	srand(time(0));
	/**** PROGRAM CODE ******/
	printf("\n************************************");
	printf("\n* Anonymous Crypto2 SBox Generator *\n\n");


	/* initialise array wit straight numbering */  
	for(i = 0; i < SBOX_ROWS; i++) {
		for(j = 0;  j < SBOX_COLS; j++) {
		  tmp[i][j] = i;
		}
	}

	printf("* before conversion\n");
	printSBox();

	for(i = 0; i < CRYPTO2_KEY_SIZE; i++) {
		createCol( i % 8 );
	}

	printf("* created first table\n");
	printSBox();

	int solution = 0;
	start = 0;

	while(solution == 0) {
	/* now check if the values are not repeating per line and column */
	for(k = 0; k < SBOX_ROWS; k++) {
	  memset(check, 0, sizeof(check));
	  for(j = 0; j <= start; j++) {
		solution = 1;
		x = tmp[k][j];
		if(x >= sizeof(check)) {
		  printf("ERROR x = %X at row %i, col: %i!\n", x, k, j);
		  return -1;
		}
		else if(check[x] == 1) {
		  solution = 0;
		  j = 99;
		  k = CRYPTO2_KEY_SIZE;
		  break;
		}
		else {
		  check[x] = 1; /* this number exists */
		}
	  }
	}
	if(solution == 1) {
	  printf("did column %i\n", j);
	  start++;
	  if(start == 8) {
		start = 0;
		printf("all done!\n");
		break;
	  }
	  else {
		solution = 0;
	  }
	}
	createCol(start);
	printf("At col %i\n", start);
	printSBox();
	}

	printf("\n*************************************");
	printf("\n* processing completed. Final SBox: *");
	printf("\n*************************************\n\n");
	printSBox();


	/* print out to file */
	FILE *f;

	f = fopen(SBOX_FILENAME, "w+b");
	if(f == NULL) {
	  printf("*** ERROR: creating sbox file!\r\n");
	  return -1;
	}
	/* first header */


	fprintf(f, "#ifndef _CRYPTO_SBOX_H_\n");
	fprintf(f, "#define _CRYPTO_SBOX_H_\n\n"); 
	fprintf(f, "/* S-Box lookup table */\n");
	fprintf(f, "#define CRYPTO_SBOX_VALUES { \\\r\n");

	u16 *pArray;
	pArray = (u16*)tmp;

	for(i = 0; i < 16; i++) {
		fprintf(f, "    0x");
		for(j = 0;  j < 8; j++) {
		  fprintf(f, "%X", *pArray);
		  pArray++;
		}
		fprintf(f, ", \\\r\n");
	}
	fprintf(f, "};\r\n");
	fprintf(f, "#endif /*_CRYPTO_SBOX_H_*/\n\n"); 
	fclose(f);

	printf("Your SBox lookup table can be you found in %s\n", SBOX_FILENAME);

	return 0;
}

/* EOF */
