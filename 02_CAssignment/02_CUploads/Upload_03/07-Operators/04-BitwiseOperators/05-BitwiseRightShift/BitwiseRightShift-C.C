#include <stdio.h>

int main(void)
{
	// function prototypes
	void PrintBinaryFormOfNumber(unsigned int);
	
	//variable declarations
	unsigned int a;
	unsigned int num_bits;
	unsigned int result;
	
	//code
	printf("\n\n");
	printf("Enter An Integer = ");
	scanf("%u", &a);
	
	printf("\n\n");
	printf("By How many bits do you want to shift A = %d To the right ? ", a);
	scanf("%u", &num_bits);
	
	printf("\n\n\n\n");
	result = a >> num_bits;
	printf("Bitwise RIGHT-SHIFTing A = %d by %d bits \nGives the result = %d (Decimal).\n\n", a, num_bits, result);
	
	PrintBinaryFormOfNumber(a);
	PrintBinaryFormOfNumber(result);
	
	return(0);
}

void PrintBinaryFormOfNumber(unsigned int decimal_number)
{
	//variable declarations
	unsigned int quotient, remainder;
	unsigned int num;
	unsigned int binary_array[8];
	int i;
	
	//code 
	for(int i = 0; i < 8; i++)
	{
		binary_array[i] = 0;
	}
	
	printf("The Binary Form of The Decimal Integer %d Is\t=\t", decimal_number);
	num = decimal_number;
	i = 7;
	while(num != 0)
	{
		quotient = num / 2;
		remainder = num % 2;
		binary_array[i] = remainder;
		num = quotient;
		i--;
	}
	
	for(i = 0; i < 8; i++)
		printf("%u", binary_array[i]);
	
	printf("\n\n");
}
