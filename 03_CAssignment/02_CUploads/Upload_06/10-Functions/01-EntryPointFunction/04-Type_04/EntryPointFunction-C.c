#include <stdio.h> // 'stdio.h' contains declaration of 'printf()'

// Entery-pont function => main() => Valid return type (int) and 2 Parameters (int argc, char *argv[])

int main(int argc, char *argv[])
{
	//variable declarations
	int i;
	
	//code
	printf("\n\n");
	printf("Hello World !!!\n"); //Library Function
	printf("Number of Command Line Arguments = %d\n\n", argc);
	
	printf("Command Line Arguments Number Passed to this program Are : \n\n");
	
	for(i = 0; i < argc; i++)
	{
		printf("Command Line Argument Number %d = %s\n", (i + 1), argv[i]);
	}
	printf("\n\n");
	return(0);
}
