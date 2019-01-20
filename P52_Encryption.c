/*  
	Author: Craig Hulsebus
	Description: P52 Encryption Algorithm 
	Date: 09/30/2018
*/


#include <stdio.h>
#include <stdlib.h>

static unsigned leftRotate(unsigned x, unsigned n) {
	return ((x << n) | (x >> (32 - n)));
}

void P52(unsigned s[12]) {
	int i, j;
	unsigned temp;
	for (i = 0; i < 24; i++) {
		for(j = 0; j < 4; j++) {
			unsigned x = leftRotate(s[j], 24);
			unsigned y = leftRotate(s[4+j], 9);
			unsigned z = s[8+j];
			s[8+j] = x ^ (z << 1) ^ ((y & z) << 2);
			s[4+j] = y ^ x ^ ((x | z) << 1);
			s[j] = z ^ y ^ ((x & y) << 3);
		}
		if ((i % 4) == 0) {
			temp = s[0];
			s[0] = s[1];
			s[1] = temp;
			temp = s[2];
			s[2] = s[3];
			s[3] = temp;
			s[0] = s[0] ^ 0x9e377900 ^ (24-i);
		}
		else if ((i % 4) == 2) {
			temp = s[0];
			s[0] = s[2];
			s[2] = temp;
			temp = s[1];
			s[1] = s[3];
			s[3] = temp;
		}
	}
	for (i = 0; i < 12; i++){
		printf("%x\n", s[i]);
	}
}		
