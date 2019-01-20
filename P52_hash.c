/**
 * @file P52_hash.c
 * @author Craig Hulsebus
 * @date 30 October 2018
 * @description Hashing algorithm that utilizes P52 encryption.
 * @execute gcc -Wall P52_Encryption.c P52_hash.c -o P52_hash 
 */

#include <stdio.h>
#include <stdlib.h>
#include <emmintrin.h>
#include <string.h>


void P52(unsigned s[12]);	

void P52_hash(unsigned char m[], unsigned mbytes, unsigned char res[], unsigned rbytes) {

	unsigned char s[48] = {0};	  // Current state block, size of 48 bytes. Unsigned = 4 bytes. 
	int i;						  // increment variable to keep track of the position of the message block 'm'

							/* Absorbtion Phase */	
	i = 0;
	while ((mbytes - i) >= 16) {   
		for (unsigned j = 0; j < 16; j++) {
			s[j] ^= m[i+j];
		}
		P52((unsigned *)s);			// feed block 's' into P52  
		i += 16;					// increment to next 16 bytes in message block m
	}

	/* Apply padding scheme 10*1 to block */
	for (unsigned j = 0; j < (mbytes - i); j++) {	
		s[j] ^= m[i+j];
	}
	s[(mbytes - i)] ^= 0x80;		// xor padding '1' with first bit of block 's'
	s[15] ^= 1;					// xor padding '1' with last bit of  block 's'
	P52((unsigned *)s);			// feed block 's' into P52
	
	
							/* Squeeze Phase */
	i = 0;
	while ((rbytes - i) >= 16) {
		memcpy(res, s, 16);			// copy 16 bytes of block 's' into 'res'
		P52((unsigned *)s);			// feed block 's' into P52
		i += 16;					// increment to next 16 bytes in message block m
	}
	/* if message tail is shorter than block size, 16 bytes */	
	memcpy(res, s, rbytes-i);   // copy remaining bytes of block 's' into 'res'
		
}
