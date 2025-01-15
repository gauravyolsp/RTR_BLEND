#include <stdio.h>

int main(void)
{
	//variable declarations
	int a, b, p;
	
	//code
	a = 9;
	b = 30;
	p = 30;
	
	// *** FIRST if-else PAIR ***
	printf("\n\n");
	if(a < b)
	{
		printf("Entering first if-block...\n\n");
		printf("A is less than B !!!\n\n");
	}
	else
	{
		printf("Entering first else-block...\n\n");
		printf("A is Not Less than B !!!\n\n");
	}
	printf("First if-else pair Done !!!\n\n");
	
	// *** SECOND if-else PAIR ***
	printf("\n\n");
	if(b != p)
	{
		printf("Entering Second if-block...\n\n");
		printf("B is NOT equal to p  !!!\n\n");
	}
	else
	{
		printf("Entering Second else-block...\n\n");
		printf("B is equal to P !!!\n\n");
	}
	printf("Second if-else Pair Done !!!\n\n");
	
	return(0);
}