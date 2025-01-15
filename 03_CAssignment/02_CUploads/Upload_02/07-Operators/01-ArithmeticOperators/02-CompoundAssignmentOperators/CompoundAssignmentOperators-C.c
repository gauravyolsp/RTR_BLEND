#include <stdio.h>

int main(void)
{
	//variable declarations
	int a;
	int b;
	int x;
	
	//code
	printf("\n\n");
	printf("Enter A Number : ");
	scanf("%d", &a);
	
	printf("\n\n");
	printf("Enter Another Number : ");
	scanf("%d", &b);
	
	printf("\n\n");
	
	// *** Since, In All The Folowwing 5 cases, the Operand on the left 'a' Is Getting Repeated Immeiately on the Right (e.g : a = a + b or a = a - b), 
	// We are using compound assignment operators +=, -=, *=, /= and %=
	
	//Sincem, 'a' will be assigned the value of (a + b) at the expression (a += b), we must save the original values of 'a' to another variable (x)
	
	x = a;
	a += b; // a = a + b
	printf("Addition of A = %d And B = %d Gives %d.\n", x, b, a);
	
	x = a;
	a -= b; // a = a - b
	printf("Subtraction of A = %d and B = %d Gives %d.\n", x, b, a);
	
	x = a;
	a *= b; // a = a * b
	printf("Multiplication of A = %d And B = %d Gives %d.\n", x, b, a);
	
	x = a;
	a /= b; // a = a / b
	printf("Division of A = %d and B = %d Gives Quotient %d.\n", x, b, a);
	
	x = a;
	a %= b; // a = a % b
	printf("Division of A = %d and B = %d Gives Remainder %d.\n", x, b, a);
	
	printf("\n\n");
	
	return(0);
}