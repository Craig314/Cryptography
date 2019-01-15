/*  
	Author: Craig Hulsebus
	Description: Finds the factorial of a user entered number.
*/

#include<stdio.h>
int main(){
  int i, j = 1, num;
 
  printf("Enter a number: ");
  scanf("%d",&num);
 
  for(i = 1; i <= num; i++) {
      f = f * i;
  }
  printf("Factorial of %d is: %d", num, f);
  return 0;
}