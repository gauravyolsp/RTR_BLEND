#include <stdio.h>
int main(void)
{
	//variable declarations
	int i, j;
	
	//code
	printf("\n\n");
	
	printf("Outer Loop Prints odd Numbers between 1 and 10.\n\n");
	printf("Inner Loop prints even numbers between 1 and 10 for every odd number printed by Outer Loop.\n\n");
	
	// condition for a number to be even number => division of number by 2 leaves no remainder (remainder = 0)
	// condition for a number to be odd number => division of number by 2 leaves remainder (remainder = 1(usually))
	
	for(i = 1; i<= 10; i++)
	{
		if(i % 2 != 0) // if number (i) is Odd
		{
			printf("i = %d\n", i);
			printf("----------\n");
			
			for(j = 1; j <= 10; j++)
			{
				if(j % 2 == 0) //if Number (j) is even...
				{
					printf("\tj = %d\n", j);
				}
				else //if Number (j) is Odd...
				{
					continue;
				}
			}
			printf("\n\n");
		}
		else // If Number (i) Is Even...
		{
			continue;
		}
	}
	
	printf("\n\n");
	
	return(0);
}