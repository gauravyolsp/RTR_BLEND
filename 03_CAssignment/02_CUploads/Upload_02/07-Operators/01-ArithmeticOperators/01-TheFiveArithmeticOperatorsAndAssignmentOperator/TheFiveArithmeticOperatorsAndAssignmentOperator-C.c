#include <stdio.h>

int main(void)
{
	//variable declarations
	int a;
	int b;
	int result;
	
	//code
	printf("\n\n");
	printf("Enter A Number : ");
	scanf("%d", &a);
	
	printf("\n\n");
	printf("Enter Another Number : ");
	scanf("%d", &b);
	
	printf("\n\n");
	
	// *** The Following are the 5 Arithmetic operators +, -, *, / and % ***
	// *** Also, The Resultants of the Arithmetic Operations In All the below Five cases have been Assigned to the variable 'result' using the Assignment Operator (=) ***
	
	result = a + b;
	printf("Addition of A = %d And B = %d Gives %d.\n", a, b, result);
	
	result = a - b;
	printf("Subtraction of A = %d and B = %d Gives %d.\n", a, b, result);
	
	result = a * b;
	printf("Multiplication of A = %d And B = %d Gives %d.\n", a, b, result);
	
	result = a / b;
	printf("Division of A = %d and B = %d Gives Quotient %d.\n", a, b, result);
	
	result = a % b;
	printf("Division of A = %d and B = %d Gives Remainder %d.\n", a, b, result);
	
	printf("\n\n");
	
	return(0);
}