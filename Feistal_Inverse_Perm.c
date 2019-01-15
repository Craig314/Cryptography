/*
	Author: Craig Hulsebus
	Description: Feistal structure to inverse, Feistal_Perm.c, permutation function.
*/

#include <stdio.h>
#include <stdlib.h>

int unsigned inverse_perm(unsigned y) {
	unsigned hi = y >> 16; // hi initialized from high 16 bits of x
	unsigned lo = y & 0xFFFF; // lo initialized from low 16 bits of x
	lo = lo ^ (hi * hi); // Feistel step: lo = lo xor f(hi)
	hi = hi ^ (lo * lo); // Feistel step: hi = hi xor f(lo)
	y = (hi >> 16) | (lo & 0xFFFF); // Reform x from low 16 bits of hi and low 16 bits of lo
	printf("%d", y);

}


int main() {

  inverse_perm(6563610);

  return 0;
}
