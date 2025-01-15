#include <stdio.h>

int main(void)
{
	//variable declarations
	int num_month;
	
	//code 
	printf("\n\n");
	
	printf("Enter Number of Month (1 to 12) : ");
	scanf("%d", &num_month);
	
	printf("\n\n");
	
	// IF - ELSE - IF LADDER BEGINS FROM HERE...
	if(num_month == 1) 
		printf("Month Number %d Is JANUARY !!!\n\n", num_month);
	
	else if(num_month == 2)
		printf("Month Number %d Is FEBRUARY !!!\n\n", num_month);

	else if(num_month == 3) 
		printf("Month Number %d Is MARCH !!!\n\n", num_month);
		
	else if(num_month == 4)
		printf("Month Number %d Is APRIL !!!\n\n", num_month);

	else if(num_month == 5) 
		printf("Month Number %d Is MAY !!!\n\n", num_month);

	else if(num_month == 6)
		printf("Month Number %d Is JUNE !!!\n\n", num_month);

	else if(num_month == 7) 
		printf("Month Number %d Is JULY !!!\n\n", num_month);

	else if(num_month == 8)
			printf("Month Number %d Is AUGUST !!!\n\n", num_month);

	else if(num_month == 9) 
		printf("Month Number %d Is SEPTEMBER !!!\n\n", num_month);
		
	else if(num_month == 10)
		printf("Month Number %d Is OCTOBER !!!\n\n", num_month);

	else if(num_month == 11) 
		printf("Month Number %d Is NOVEMBER !!!\n\n", num_month);

	else if(num_month == 12)
		printf("Month Number %d Is DECEMBER !!!\n\n", num_month);

	else
		printf("Invalid month number %d Entered !!! Please try again...\n\n", num_month);
	
	printf("If - else if - Else Ladder Complete !!!\n");
	
	return(0);
}
