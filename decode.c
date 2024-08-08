/****************************************************************************
 *
 *  Description: A C implementation of the Serpent encryption algorythmus
 *
 *  Author: WE ARE ANONYMOUS; EXPECT US. 
 *
 *  Copyrights. None. This is purely public domain
 *
 *  File: decode.c
 *
 *  Description: Decryption front end
 *
 *  Version 1.0
 *
 *  Project: Paranoid Serpent encryption tool
 *
 ****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

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


static void print_usage(char *progname)
{
	printf("Usage: %s [-w] 'encrypted input file' [unencrypted output file]\n\n", progname);
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
	u32 arg;
	u32 crc;
	bool use_filewrite;
	bool use_hdrfname;
	u8 buffer[32];
	HEADER_T hdr;
	FILE_T infile;
	FILE_T outfile;
	
	arg = 0;
	use_filewrite = false;
	use_hdrfname = true;
	
	/***** Paranoia checks *****/
	if(sizeof(infile.name) != sizeof(hdr.fname)) {
		ERROR("Program contains a fault!");
	}
	
	printf("\nS.A.P.E. Serpent Algorythm based Paranoia [En|De]crypter V%u.%u\n\n", VERSION, RELEASE);
	
	if(argc < 1) {
		print_usage(argv[arg]);
		ERROR("no input file provided!");

	}
	
	arg++; /* move on to the next cmd line argument as the first is the program name itself */
	argc--;

	for(i = arg; i <= argc; i++) { /* check it is some kind of extra parameter starting with '-' */
		if(argv[arg][0] == '-') {
			switch (argv[arg][1]) {
				case 0: 
				ERROR("parameter invalid!");
				break;
				case 'w': /* we got -w, write the decrypted output to a file */
				use_filewrite = true;
				break;
			}
			arg++;
			argc--;
		}
	}
	
	if(argc == 2) { /* if we write to a given output file, 2 names must be provided as parameters */
		use_hdrfname = false;	
	}
	else  if(argc == 1) { /* ony one file name as parameter, we use the filename from decrypted header */
		use_hdrfname = true;
	}
	else if(argc == 0) { /* not arguments? does not work! */
		ERROR("no input filename provided!");
	}
	else {
		ERROR("Too many parameters given!");
	}

#if 0
    for (int i=0; i<argc; i++) {
        printf("%s: argv[%d] = '%s'\n", __FUNCTION__, i, argv[i]);
    }
#endif

	/**** input file name processing ****/
	
	/* get the length of the filename anc check if it is too long */
	size = my_strlen(argv[arg]);
	if(size >= sizeof(hdr.fname)) {
		ERROR("Input file name is longer than %lu chars which is not supported!", sizeof(hdr.fname)-1);
	}
	my_strcpy(infile.name, argv[arg], sizeof(infile.name));
	arg++;

	/* process file content */
	LOG("Open '%s' for decryption", infile.name);
	infile.stream = fopen(infile.name, "rb");
	
	if(infile.stream == NULL) {
		ERROR("Could not open input file %s", infile.name);
	}
	
	if(fread((u8*)&hdr, 1, sizeof(hdr), infile.stream) != sizeof(hdr)) {
		ERROR("input file is corrupt!");
	}
	
	fprintf(stdout, "\nPlease enter the secret password that protects this file: ");
	scrambleByPwd(m_key, sizeof(m_key));
	
	CRYPTO2_init(m_key, &m_keyring);

	/* decrypt file header */
	CRYPTO2_decrypt((u8*)&hdr, sizeof(hdr), &m_keyring);
	
	if(!isValidString(hdr.fname)) {
		ERROR("decrypted filename is invalid!");
	}
	
	if(use_hdrfname) { /* use filename of the decrypted header */
		my_strcpy(outfile.name, hdr.fname, sizeof(outfile.name));
	}
	else { /* use command line output file name */
		size = my_strlen(argv[arg]);
		if(size >= sizeof(hdr.fname)) {
			ERROR("Output file name is longer than %lu chars which is not supported!", sizeof(hdr.fname)-1);
		}
		my_strcpy(outfile.name, argv[arg], sizeof(outfile.name));
	}


	LOG("Output filename: %s", hdr.fname);
	LOG("Reported file size: %u, CRC: 0x%08X", hdr.fsize, hdr.crc);

	/* set the target fo the decrypted data */
	if(use_filewrite) {
		outfile.stream = fopen(outfile.name, "w");
	}
	else {
		outfile.stream = stdout;
	}

	if(outfile.stream == NULL) {
		ERROR("could not open output file '%s'", outfile.name);
	}

	if(!use_filewrite) { /* print separator if it goes to stdout */
		LOG("\n______________________________ CONTENT ________________________________________");
	}
	written = 0;
	filesize = hdr.fsize;
	crc = 0;
	while(filesize > 0) {
		my_memclr(buffer, 32);
		size = fread(buffer, 1, 32, infile.stream);
		if(size == 0) {
			ERROR("unexpected end of file!");
		}
		if(filesize >= size) {
			filesize -= size;
		}
		else {
			size = filesize;
			filesize = 0;
		}
		CRYPTO2_decrypt(buffer, sizeof(buffer), &m_keyring);
		crc = crc32(crc, buffer, size);
		if(!use_filewrite) {
			for(i = 0; i < sizeof(buffer); i++) { /* remove non printable characters */
				
			}
		}
		written += fwrite(buffer, 1, size, outfile.stream);
	}

	if(!use_filewrite) { /* print separator if it goes to stdout */
		LOG("____________________________ END OF CONTENT ___________________________________");
	}
	LOG("%u bytes written. CRC difference: %u", written, crc - hdr.crc);

	fclose(infile.stream);
	fclose(outfile.stream);
}

/* EOF */
