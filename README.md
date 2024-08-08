**NAME**

S.A.P.E. Serpent Algorythm based Paranoia [En|De]crypter

**SYNOPSIS**

sape-enc 'input file' 'output file'
sape-dec [-w] 'input file' ['output file']

**DESCRIPTION**

A simple plain C implementation of the Serpent block cypher.

**License**
Public domain. Use, spread, edit this code as you like but at your own risk. 

Read details about the algorithm in Wikipedia: https://en.wikipedia.org/wiki/Serpent_(cipher)

	-w	write file to disk. If no output file name is defined, the original filenam from encrypted header is used.

**INSTALLATION**

	Extract all files into a clean folder. Be sure you have gcc and the make tools installed.

	Run 'make all'

	To clean the build run 'make clean'

	You should now get two new executable binaries in your working folder: sape-enc and sape-dec
	They can be copied to /usr/bin for example to use them system-wide.

**EXAMPLES**

	sape-enc mytext.txt myencryptedtext.enc
		after start, enter your private password and it encrypts your file

	sape-dec -w myencryptedtext.enc
		after start, enter your private password and the original file is written to the working directoy

	sape-dec -w myencryptedtext.enc ~/text.txt
		after start, enter your private password and the original file is written as text.txt to the home directory

**AUTHOR**
	Written by Tobias Weber 2022


**MORE DETAILS**

	The encryption works as follows:

	Input data is put into a matrix. The matrix lines are rotated, then part of the cells of the matrix are replaced by other symbols (so called substitution) through a lookup table (SBox). This habens a 31 times in a loop and creates a really quite high entropy which makes serpent so hard to crack.

	There are 4 elements that define how data is encrypted. 

	1. The Key. It is a arbitrary 128 Bit binary key organised in an array of 32 bytes (32*8 = 128)

	This key can be found in "crypto_key.h".

	2. The SBox (substitution box). This 512 bit lookup table is used to replace cell content of the matrix.

	The SBox lookup table can be found in "crypto_sbox.h"

	3. The substitution offset. The position of which the 4 bits of a line in the matrix are exchanged to a value from the SBox.

	It is defined in crypto2.h as "SBOX_SHIFT" and is by default 13.

	4. The user provided password that must be entered manually wich each en/decrpyt. It is never stored anywhere on the computer.

	If only one of the above parameters differ between encryption and decryption function only 1 bit, the encrypted data cannot be decrypted.

	The current SBox and Key are default values and shall not be used for any encryption.


