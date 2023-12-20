#include <time.h>
#include <stdio.h>
#include <x86intrin.h>
#include "simd.h"

long long int sum(int vals[NUM_ELEMS]) {
	clock_t start = clock();

	long long int sum = 0;
	for(unsigned int w = 0; w < OUTER_ITERATIONS; w++) {
		for(unsigned int i = 0; i < NUM_ELEMS; i++) {
			if(vals[i] >= 128) {
				sum += vals[i];
			}
		}
	}
	clock_t end = clock();
	printf("Time taken: %Lf s\n", (long double)(end - start) / CLOCKS_PER_SEC);
	return sum;
}

long long int sum_unrolled(int vals[NUM_ELEMS]) {
	clock_t start = clock();
	long long int sum = 0;

	for(unsigned int w = 0; w < OUTER_ITERATIONS; w++) {
		for(unsigned int i = 0; i < NUM_ELEMS / 4 * 4; i += 4) {
			if(vals[i] >= 128) sum += vals[i];
			if(vals[i + 1] >= 128) sum += vals[i + 1];
			if(vals[i + 2] >= 128) sum += vals[i + 2];
			if(vals[i + 3] >= 128) sum += vals[i + 3];
		}

		//This is what we call the TAIL CASE
		//For when NUM_ELEMS isn't a multiple of 4
		//NONTRIVIAL FACT: NUM_ELEMS / 4 * 4 is the largest multiple of 4 less than NUM_ELEMS
		for(unsigned int i = NUM_ELEMS / 4 * 4; i < NUM_ELEMS; i++) {
			if (vals[i] >= 128) {
				sum += vals[i];
			}
		}
	}
	clock_t end = clock();
	printf("Time taken: %Lf s\n", (long double)(end - start) / CLOCKS_PER_SEC);
	return sum;
}

long long int sum_simd(int vals[NUM_ELEMS]) {
	clock_t start = clock();
	__m128i _127 = _mm_set1_epi32(127);		// This is a vector with 127s in it... Why might you need this?
	long long int result = 0;				   // This is where you should put your final result!
	/* DO NOT DO NOT DO NOT DO NOT WRITE ANYTHING ABOVE THIS LINE. */
	
	for(unsigned int w = 0; w < OUTER_ITERATIONS; w++) {

		__m128i sum_vector = _mm_setzero_si128();		// declaring and initialising the sum vector
		__m128i buffer, compare;						// these could also be initialised with zeroes, but
														// not absolutely necessary, they are written over in
														// the inner for-loop

		int p[4]; 										// I used "int *p;" here once, which caused a bus error

		for (unsigned int i = 0; i < NUM_ELEMS / 4 * 4; i += 4)
		{
			buffer = _mm_loadu_si128((__m128i *)(vals + i));	// stores the next 4 elements of the array
			
			compare = _mm_cmpgt_epi32(buffer, _127);			// if the element of the buffer vector is 
																// greater than 127, than compare will have
																// ones at the same place, if otherwise then zeroes
			
			buffer = _mm_and_si128(buffer, compare);			// eliminating the values in the buffer, that
																// are less than 128 (I actually don't know why
																// we do that)
			
			sum_vector = _mm_add_epi32(buffer, sum_vector);		// adding to the sum_vector

		}

		_mm_storeu_si128((__m128i *)p, sum_vector);		// sum_vector will be zeroed out once we go into the
														// outer for-loop again, so we need to save the result now

		for (unsigned int i = 0; i < 4; i++)
			result += p[i];

		// Tail case:		
		for (unsigned int i = NUM_ELEMS / 4 * 4; i < NUM_ELEMS; i++)
		{
			if (vals[i] >= 128) result += vals[i];
		}
		
	}
	clock_t end = clock();
	printf("Time taken: %Lf s\n", (long double)(end - start) / CLOCKS_PER_SEC);
	return result;
}

long long int sum_simd_unrolled(int vals[NUM_ELEMS]) {
	clock_t start = clock();
	__m128i _127 = _mm_set1_epi32(127);
	long long int result = 0;
	for(unsigned int w = 0; w < OUTER_ITERATIONS; w++) {

		__m128i sum_vector = _mm_setzero_si128();
		__m128i buffer, compare;

		int p[4];

		// Unrolling 4 times:
		for (unsigned int i = 0; i < NUM_ELEMS / 16 * 16; i += 16)
		{
			buffer = _mm_loadu_si128((__m128i *)(vals + i));
			compare = _mm_cmpgt_epi32(buffer, _127);
			buffer = _mm_and_si128(buffer, compare);
			sum_vector = _mm_add_epi32(buffer, sum_vector);

			buffer = _mm_loadu_si128((__m128i *)(vals + i + 4));
			compare = _mm_cmpgt_epi32(buffer, _127);
			buffer = _mm_and_si128(buffer, compare);
			sum_vector = _mm_add_epi32(buffer, sum_vector);

			buffer = _mm_loadu_si128((__m128i *)(vals + i + 8));
			compare = _mm_cmpgt_epi32(buffer, _127);
			buffer = _mm_and_si128(buffer, compare);
			sum_vector = _mm_add_epi32(buffer, sum_vector);

			buffer = _mm_loadu_si128((__m128i *)(vals + i + 12));
			compare = _mm_cmpgt_epi32(buffer, _127);
			buffer = _mm_and_si128(buffer, compare);
			sum_vector = _mm_add_epi32(buffer, sum_vector);

		}

		_mm_storeu_si128((__m128i *)p, sum_vector);

		for (unsigned int i = 0; i < 4; i++)
			result += p[i];

		// Tail case:		
		for (unsigned int i = NUM_ELEMS / 16 * 16; i < NUM_ELEMS; i++)
		{
			if (vals[i] >= 128) result += vals[i];
		}

	}
	clock_t end = clock();
	printf("Time taken: %Lf s\n", (long double)(end - start) / CLOCKS_PER_SEC);
	return result;
}