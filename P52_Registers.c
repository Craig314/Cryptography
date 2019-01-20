/**
 * @file P52_Registers.c
 * @author Craig Hulsebus
 * @date 10 October 2018
 * @description P52 encryption with register optimization. 
 */

#include <stdio.h>
#include <stdlib.h>
#include <emmintrin.h>
#include <string.h>


void P52(unsigned s[12]) {
	unsigned temp;
	int i;
	int size = sizeof(*s);

	for (i = 0; i < 24; i++) {
		/* Loads 16 byte register called row_one with mem from a. */
		__m128i row_one = _mm_loadu_si128 ((__m128i *) (&s[0]));
		/* Loads 16 byte register called row_two with mem from b. */
		__m128i row_two = _mm_loadu_si128 ((__m128i *) (&s[4]));
		/* Loads 16 byte register called row_three with mem from c. */
		__m128i row_three = _mm_loadu_si128 ((__m128i *) (&s[8]));

		/* Left Rotate by 24 */
		row_one = _mm_or_si128 (
						_mm_slli_epi32 (row_one, 24) ,  // 24	Both return 1234
						_mm_srli_epi32 (row_one, 8)		// 8 
						);

		/* Left Rotate by 9 */
		row_two = _mm_or_si128 (
						_mm_slli_epi32 (row_two, 9),	// 9  Both return: 6789
						_mm_srli_epi32 (row_two, 23)	// 23
						);

		/* Stores register row_one into array s */
		_mm_storeu_si128 ((__m128i *) (&s[0]), row_one);
		/* Stores register row_two into array s */
		_mm_storeu_si128 ((__m128i *) (&s[4]), row_two);

		/* Create register x and set its values to those of row_one */
		__m128i x = _mm_set_epi32 (s[3],s[2],s[1],s[0]);
		/* Create register y and set its values to those of row_two */
		__m128i y = _mm_set_epi32 (s[7],s[6],s[5],s[4]);

		/* s[8] = z0 ^ y0 ^ ((x0 & y0) << 3); */
		row_one = _mm_xor_si128 (
					row_three, ( 
						_mm_xor_si128 (
							y, ( 
								_mm_slli_epi32( 
									_mm_and_si128(x, y),
									3 
								)
							)
						)
					)
				);

		/* s[4] = y0 ^ x0 ^ ((x0 | z0) << 1); */
		row_two = _mm_xor_si128 (
					y, ( 
						_mm_xor_si128 (
							x, (
								_mm_slli_epi32( 
									_mm_or_si128(x, row_three),
									1 
								)
							)
						)
					)
				);

		/* s[0] = x0 ^ (z0 << 1) ^ ((y0 & z0) << 2); */
		row_three = _mm_xor_si128 (
						x, ( 
							_mm_xor_si128 (
								_mm_slli_epi32(row_three, 1),
								_mm_slli_epi32(
									_mm_and_si128(y, row_three),
									2
								)
							)
						)
					);

		/* Stores register row_one into array s */
		_mm_storeu_si128 ((__m128i *) (&s[0]), row_one);
		/* Stores register row_two into array s */
		_mm_storeu_si128 ((__m128i *) (&s[4]), row_two);
		/* Stores register row_three into array s */
		_mm_storeu_si128 ((__m128i *) (&s[8]), row_three);

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
	
}
