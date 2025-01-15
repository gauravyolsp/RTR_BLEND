#include <stdio.h>

int main(void)
{
	//variable declarations
	int a;
	
	//code 
	printf("\n\n");
	
	a = 5;
	if(a) // Non-zero Positive Value
	{
		printf("if-block 1 : 'A' Exists And has value = %d !!!\n\n", a);
	}
	
	a = -5;
	if(a) // Non-zero Negative Value
	{
		printf("if-block 2 : 'A' Exists and has value = %d !!!\n\n", a);
	}
	
	a = 0;
	if(a) // zero value
	{
		printf("if-block 3 : 'A' Exists and has value = %d !!!\n\n", a);
	}
	
	printf("All three if-statements are Done !!!\n\n");
	return(0);
}
