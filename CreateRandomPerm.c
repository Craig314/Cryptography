/*
	Author: Craig Hulsebus
	Description: Creates a random permutation by passing in an integer. 
*/

#include <stdio.h>
#include <stdlib.h>

int* createRandomPermutation(int n) {
	int i;	
	//Determine size of the array
	int size = sizeof(a) / sizeof(a[0]);			
	int a[n], b[n];																
	//Initialze array
	for (i = 0; i < size; i++) {					
		b[i] = i;
	}
	for (i = 0; i < size; i++) {
		//Generate random number between 0 and n-1.
    	int j = i+rand() % (n-i+1);		
    	//Swap elements				
		int hold = b[i];							
		b[i] = b[j];						
		b[j] = hold;
		//Elemnts in array a are equal to array b.	
		a[i] = b[i];								
	    //Print out Element number of array a with its value.
		printf("Element at a[%d]: %d\n", i, a[i]);
	}
}

int main(void) {

  createRandomPermutation(10);
  return 0;
}