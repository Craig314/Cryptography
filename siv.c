/**
 * @file siv.c and hw5_lib.o
 * @library provided by Ted Krovetz (Sacramento State Professor)
 * @author Craig Hulsebus
 * @date 11 November 2018
 * @description SIV Encryption/Decryption Scheme Algorithm
 * @note hw_lib.o contains P52_BC, P52_CTR, and the poly61 functions
 */

#include <stdio.h>
#include <stdlib.h>
#include <emmintrin.h>
#include <string.h>
#include <inttypes.h>

// Computes (key||0*) xor P52(in xor (key||0*)).
void P52_BC(unsigned kbytes, unsigned char key[kbytes],     // 16 or 32 only
            unsigned char in[48], unsigned char out[48]);   // in == out okay

// Encrypt p into c. Copy of iv gets 0* padded and big-endian incremented
void P52_CTR(unsigned kbytes, unsigned char key[kbytes],  // Upto 48 bytes
             unsigned ivbytes, unsigned char iv[ivbytes], // Upto 48 bytes
             unsigned inbytes, unsigned char in[inbytes], // in == out is okay
             unsigned char out[inbytes]);

// k and result are less than 2^61-1 and read and written as numbers.
// Result is approx (mbytes/2^63)-AU when k is random in [0,2^61-1)
uint64_t poly61(unsigned char m[], unsigned mbytes, uint64_t k);

// decrypt c into p. Return non-zero iff t authenticates p
int pcm_siv_decrypt(unsigned kbytes, unsigned char k[kbytes],  // 16 or 32
                    unsigned nbytes, unsigned char n[nbytes],  // 0 to 16
                    unsigned cbytes, unsigned char c[cbytes],
                    unsigned char p[cbytes], unsigned char t[16]);



// Encrypt p into c and write authentication tag to t
void pcm_siv_encrypt(unsigned kbytes, unsigned char k[kbytes], // Must be 16 or 32
					 unsigned nbytes, unsigned char n[nbytes], // Must be 0 to 16
					 unsigned pbytes, unsigned char p[pbytes],	
					 unsigned char c[pbytes], unsigned char t[16]) {

	uint64_t hashkey1[1], hashkey2[1];
	unsigned char tempIn[48] = {[0 ... 47] = 0XFF}; 	// 48 bytes of 0XFF
	unsigned char sivTemp[48];							// 48 bytes char array to hold siv output
	unsigned char siv[16];								// 16 bytes from sivTemp
	unsigned char keys[48];								// Key output from P52-BC
	unsigned sivBytes = sizeof(siv);

	// Run key of size kbytes through P52-BC
	P52_BC(kbytes, k, tempIn, keys);	

	memcpy(&hashkey1, keys, 8);				// Copy 8 bytes from keys to hashkey1
	hashkey1[0] = hashkey1[0] >> 4;			// Makes high 4 bits zero
	memcpy(&hashkey2, keys+8, 8);			// Copy 8 bytes from keys+8 to hashkey2
	hashkey2[0] = hashkey2[0] >> 4;			// Makes high 4 bits zero

	// Run plaintext through Poly61 with the generated key.
	// Do this twice and compare hash1 and hash2 for collision
	uint64_t hash1 = poly61(p, pbytes, *hashkey1); 
	uint64_t hash2 = poly61(p, pbytes, *hashkey2); 
	if (hash1 == hash2) {
		printf("Collison has occured, program is closing.");
		exit(0);
	}

	// Compute SIV by running the key and the concatination through P52-BC
	unsigned char inArr[48] = {0};
	memcpy(&inArr, &hash1, 8);				// copy hash1 into 'inArr'
	memcpy(&inArr[8], &hash2, 8);			// copy hash2 into 'inArr'
	memcpy(inArr+8+8, n, nbytes);			// copy nonce into 'inArr'
	memset(inArr+8+8+nbytes, 0x80, 1);		// add padding 10* into 'inArr'
	
	// Run concatinated array 'inArr[48]' through P52-BC
	P52_BC(kbytes, k, inArr, sivTemp); 

	memcpy(siv, sivTemp, 16);		// Copy first 16 bytes of sivTemp[48] into siv[16]

	memcpy(t, siv, 16);				// The synthetic IV serves as authentication tag
	
	// Run newly created siv along with key and plaintext through P52-CTR
	P52_CTR(kbytes, k, sivBytes, siv, pbytes, p, c);
	
}
