#include <stdio.h> // for printf()
#include <conio.h> // for getch()

int main(void)
{
	//variable declaractions
	int a, b;
	int result;
	
	char option, option_division;
	
	//code 
	printf("\n\n");
	
	printf("Enter value for 'A' : ");
	scanf("%d", &a);
	
	printf("Enter value for 'B' : ");
	scanf("%d", &b);
	
	printf("Enter Option In Character : \n\n");
	printf("'A' or 'a' For Addition : \n");
	printf("'S' or 's' For Subtraction : \n");
	printf("'M' or 'm' For Multiplication : \n");
	printf("'D' or 'd' For Division : \n");
	
	printf("Enter Option : ");
	option = getch();
	
	printf("\n\n");
	
	if(option == 'A' || option == 'a')
	{
		result = a + b;
		printf("Addition of A = %d and B = %d Gives result %d !!!\n\n", a, b, result);
	}
	else if(option == 'S' || option == 's')
	{
		if(a >= b)
		{
			result = a - b;
			printf("Subtraction of B = %d and A = %d Gives result %d !!!\n\n", b, a, result);
		}
		else
		{
			result = b - a;
			printf("Subtraction of A = %d and B = %d Gives result %d !!!\n\n", a, b, result);
		}
	}
	else if(option == 'M' || option == 'm')
	{
		result = a * b;
		printf("Multiplication of A = %d and B = %d Gives result %d !!!\n\n", a, b, result);
	}
	else if(option == 'D' || option == 'd')
	{
		printf("Enter Option In Character : \n\n");
		printf("'Q' or 'q' or '/' for Quotient upon Division : \n");
		printf("'R' or 'r' or '/' for Remainder upon Division : \n");
		
		printf("Enter Option : ");
		option_division = getch();
		
		printf("\n\n");
		
		if(option_division == 'Q' || option_division == 'q' || option_division == '/')
		{
			if(a >= b)
			{
				result = a / b;
				printf("Division of A = %d and B = %d Gives Quotient = %d !!!\n\n", a, b, result);
			}
			else
			{
				result = b / a;
				printf("Division of B = %d and A = %d Gives Quotient = %d !!!\n\n", b, a, result);
			}
		}
		else if(option_division == 'R' || option_division == 'r' || option_division == '%')
		{
			if(a >= b)
			{
				result = a % b;
				printf("Division of A = %d and B = %d Gives Remainder = %d !!!\n\n", a, b, result);
			}
			else
			{
				result = b % a;
				printf("Division of B = %d and A = %d Gives Remainder = %d !!!\n\n", b, a, result);
			}
		}
		else
		{
			printf("Invalid Character %c Entered For Division !!! please try again...\n\n", option_division);
		}
	}
	else
	{
		printf("Invalid Character %c Entered !!! please try again...\n\n", option);
	}
	
	printf("If - Else If - Else Ladder Complete !!!\n");	
	
	return(0);
}