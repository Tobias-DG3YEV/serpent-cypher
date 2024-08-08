/****************************************************************************
 *
 *  Description: A C implementation of the Serpent encryption algorythmus
 *
 *  Author: WE ARE ANONYMOUS; EXPECT US. 
 *
 *  Copyrights. None. This is purely public domain
 *
 *  File: encode.c
 *
 *  Description: Encryption front end
 *
 *  Version 1.0
 *
 *  Project: Paranoid Serpent encryption tool
 *
 ****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "platform_types.h"
#include "stdfunc.h"
#include "crypto2.h"
#include "crypto_key.h"
#include "crc.h"

/****************************************************************************
*                              DEFINES
*****************************************************************************/

#define VERSION				0
#define RELEASE				1

/****************************************************************************
*                              LOCAL VARIABLES
*****************************************************************************/

u8 m_key[CRYPTO2_KEY_SIZE] = CRYPTO_KEY;
T_CRYPTOBUF m_keyring;

/****************************************************************************
*                              LOCAL FUNCTIONS
*****************************************************************************/


/****************************************************************************
* FUNC:		print_usage
* PARAM:	char *progname - name of the executable
* RET:		void
* DESC:		print program usage
*****************************************************************************/
static void print_usage(char *progname)
{
	printf("Usage: %s 'unencrypted input file' 'encrypted output file'\n\n", progname);
}

/****************************************************************************
* FUNC:		write_outfile32
* PARAM:	
* RET:		void
* DESC:		
*****************************************************************************/
static u32 write_outfile32(FILE * f, u8* pData) /* writes 32 bytes to the output file */
{
	return fwrite(pData, 1, 32, f);
}

/****************************************************************************
* FUNC:		read_infile32
* PARAM:	
* RET:		void
* DESC:		
*****************************************************************************/
static u32 read_infile32(FILE * f, u8* pData) /* writes 32 bytes to the output file */
{
	return fread(pData, 1, 32, f);
}

/****************************************************************************
*                              GLOBAL FUNCTIONS
*****************************************************************************/

/****************************************************************************
* FUNC:		main
* PARAM:	int argc, char *argv[]
* RET:		int
* DESC:		program entry function
*****************************************************************************/
int main(int argc, char * argv[])
{

	/***** DEFINITION *******/
	u32 size;
	u32 filesize;
	u32 written;
	u32 i;
	u8 buffer[32];
	HEADER_T hdr;
	time_t t;
	FILE_T infile;
	FILE_T outfile;
	
	/***** Paranoia checks *****/
	if(sizeof(infile.name) != sizeof(hdr.fname)) {
		ERROR("Program contains a fault!");
	}
	
	printf("\nS.A.P.E. Serpent Algorythm based Paranoia [En|De]crypter V%u.%u\n\n", VERSION, RELEASE);
	
	if(argc <= 1) {
		print_usage(argv[0]);
		ERROR("no input file provided!");

	}

	if(argc <= 2) {
		print_usage(argv[0]);
		ERROR("no output file provided!");
	}

#if 0
    for (int i=0; i<argc; i++) {
        printf("%s: argv[%d] = '%s'\n", __FUNCTION__, i, argv[i]);
    }
#endif
	/* get the length of the filename anc check if it is too long */

	size = my_strlen(argv[2]);
	if(size >= sizeof(hdr.fname)) {
		ERROR("Output file name is longer than %lu chars which is not supported!", sizeof(hdr.fname)-1);
	}

	size = my_strlen(argv[1]);
	if(size >= sizeof(hdr.fname)) {
		ERROR("Input file name is longer than %lu chars which is not supported!", sizeof(hdr.fname)-1);
	}

	/* prepare header */

   /* Intializes random number generator */
   srand((unsigned) time(&t));
   
   for(i = 0; i < sizeof(hdr.salt); i++) {
	   hdr.salt[i] = rand();
   }
   
	/* fill the rest of the input file name field with random numbers first to have some salt in the encryption */
	for(i = 0; i < sizeof(hdr.fname); i++) {
		if(i < size) {
			hdr.fname[i] = argv[1][i];
			infile.name[i] = argv[1][i];
		}
		else if(i == size) {
			hdr.fname[i] = 0; /* terminate the filename string */
		}
		else {
			hdr.fname[i] = rand();
		}
	}
	/* output file name */
	size = my_strlen(argv[2]);
	for(i = 0; i < sizeof(outfile.name); i++) {
		outfile.name[i] = argv[2][i];
		if(outfile.name[i] == 0) {
			break;
		}
	}
	/* filename shall not be identical */
	bool identical = true;
	size = MAX(my_strlen(infile.name), my_strlen(outfile.name));
	for(i = 0; i < size; i++) {
		if(outfile.name[i] != infile.name[i]) {
			identical = false;
			break;
		}
	}
	if(identical) {
		ERROR("Input and output shall not be identical!");
	}
	/* process file */

	LOG("Open '%s' for encryption", argv[1]);

	infile.stream = fopen(hdr.fname, "rb");
	
	if(infile.stream == NULL) {
		ERROR("Could not open input file %s", infile.name);
	}

	hdr.crc = 0;
	filesize = 0;
	do {
		size = fread(buffer, 1, sizeof(buffer), infile.stream);
		hdr.crc = crc32(hdr.crc, buffer, size);
		filesize += size;
	} while(size > 0);
	hdr.fsize = filesize;
	
	LOG("Input file is %u bytes of size.", filesize);
	LOG("CRC of the file content is 0x%08X.", hdr.crc);

	LOG("Please enter the secret password that protects this file: ");
	scrambleByPwd(m_key, sizeof(m_key));

	CRYPTO2_init(m_key, &m_keyring);

	/* encrypt header */
	CRYPTO2_encrypt((u8*)&hdr, sizeof(hdr), &m_keyring);

	outfile.stream = fopen(outfile.name, "w");
	if(outfile.stream == NULL) {
		ERROR("could not open output file '%s'", outfile.name);
	}
	written = fwrite((u8*)&hdr, 1, sizeof(hdr), outfile.stream);
	
	fseek(infile.stream, 0, SEEK_SET);

	while(filesize > 0) {
		my_memclr(buffer, 32);
		size = read_infile32(infile.stream, buffer);
		if(size == 0) {
			ERROR("unexpected end of file!");
		}
		filesize -= size;
		CRYPTO2_encrypt(buffer, sizeof(buffer), &m_keyring);
		written += write_outfile32(outfile.stream, buffer);
	}
	
	fclose(infile.stream);
	fclose(outfile.stream);
	
	LOG("%u bytes written.", written);

}

/* EOF */
