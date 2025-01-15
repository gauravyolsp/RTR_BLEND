#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

int main(int argc, char *argv[], char *envp[])
{
    // variable declarations
    int i;
    int num;
    int sum = 0;

    // code
    if(argc == 1)
    {
        printf("\n\n");
        printf("No Numbers Given for addition !!! Exiting Now... \n\n");
        printf("Usage : CommandLineArgumentsApplication <first number> <second number> ...\n\n");
        exit(0);
    }

    // *** THIS PROGRAMS ADDS ALL COMMAND LINE ARGUMENTS GIVEN IN INTEGER FORM ONLY AND OUTPUTS THE SUM ***
    // *** DUE TO USE OF atoi(), ALL COMMAND LINE ARGUMENTS OF TYPES OTHER THAN 'int' ARE OGNORED ***
    printf("\n\n");
    printf("Sum Of All Integer Command Line Arguments Is : \n\n");
    for(i = 1; i < argc; i++)
    {
        num = atoi(argv[i]);
        sum = sum + num;
    }

    printf("Sum = %d\n\n", sum);

    return(0);
}
