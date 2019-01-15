/*
	Author: Craig Hulsebus
	Description: Creates a random function by passing in an integer. 
*/

#include <stdio.h>
#include <stdlib.h>

int* createRandomFunction(int n) {
	int i;
	//Determine size of the array
	int a[n];
	int size = sizeof(a) / sizeof(a[0]);		
	//Fills array a[] with n random values ranging from 0 to n-1.
	for (i = 0; i < size; i++) {				
		a[i] = rand() % n; 		
		//Print out Element number of the array with its value.		
		printf("Element at a[%d]: %d\n", i, a[i]);	
	}
}

int main(void) {

  createRandomFunction(10);

  return 0;
}