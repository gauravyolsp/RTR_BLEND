#include <stdio.h>

int main(void)
{
	//code 
	printf("\n\n");
	printf("****************************************************************************************");
	printf("\n\n");
	
	printf("Hello World !!!\n\n");
	
	int a = 13;
	printf("Integer Decimal value of 'a' = %d\n", a);
	printf("Integer Octal value of 'a' = %o\n", a);
	printf("Integer Hexadecimal value of 'a' (Hexadecimal Letters in Lower Case) = %x\n\n", a);
	printf("Integer Hexadecimal value of 'a' (Hexadecimal Letters in Upper Case) = %X\n\n", a);
	
	char ch = 'A';
	printf("Character ch = %c\n", ch);
	char str[] = "AstroMedicomp's real time Rendering Batch";
	printf("String str = %s\n\n", str);
	
	long num = 30121995L;
	printf("Long Integer = %ld\n\n", num);
	
	unsigned int b = 7;
	printf("Unsigned Integer 'b' = %u\n\n", b);
	
	float f_num = 3012.1995f;
	printf("Floating point number with just %%f 'f_num' = %f\n", f_num);
	printf("Floating point number with just %%4.2f 'f_num' = %4.2f\n", f_num);
	printf("Floating point number with just %%6.5f 'f_num' = %6.5f\n", f_num);
	
	double d_pi = 3.14159265358979323846;
	printf("Double Precision Floating point number without Exponential = %g\n", d_pi);
	printf("Double Precision Floating point number without Exponential (Lower case) = %e\n", d_pi);
	printf("Double Precision Floating point number without Exponential (Upper case) = %E\n", d_pi);
	printf("Double Hexadecimal Value of 'd_pi' (Hexadecimal Letters in Lower Case) = %a\n", d_pi);
	printf("Double Hexadecimal Value of 'd_pi' (Hexadecimal Letters in Upper Case) = %A\n", d_pi);
	
	printf("****************************************************************************************");
	printf("\n\n");
	return(0);
}
