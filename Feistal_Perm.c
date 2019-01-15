/*
	Author: Craig Hulsebus
	Description: Feistal structure of a permutation, using X^2 mixing function.
*/

#include <stdio.h>
#include <stdlib.h>

int unsigned perm(unsigned x) {
	hi = x >> 16; // hi initialized from high 16 bits of x
	lo = x & 0xFFFF; // lo initialized from low 16 bits of x
	hi = hi ^ (lo * lo); // Feistel step: hi = hi xor f(lo)
	lo = lo ^ (hi * hi); // Feistel step: lo = lo xor f(hi)
	x = (hi << 16) | (lo & 0xFFFF); // Reform x from low 16 bits of hi and low 16 bits of lo
	return x;
}


int main() {

  createRandomPermutation()

  return 0;
}
