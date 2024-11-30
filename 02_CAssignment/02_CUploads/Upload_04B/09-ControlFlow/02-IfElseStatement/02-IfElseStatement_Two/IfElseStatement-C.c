#include <stdio.h>

int main(void)
{
	//variable declarations
	int age;
	
	//code
	printf("\n\n");
	printf("Enter Age : ");
	scanf("%d", &age);
	printf("\n\n");
	if(age >= 18)
	{
		printf("Entering if-block...\n\n");
		printf("You are Eligible for Voting !!\n\n");
	}
	else
	{
		printf("Entering else-block...\n\n");
		printf("You are NOT Eligible for Voting !!!\n\n");
	}
	
	printf("Bye !!!\n\n");
	return(0);
}
