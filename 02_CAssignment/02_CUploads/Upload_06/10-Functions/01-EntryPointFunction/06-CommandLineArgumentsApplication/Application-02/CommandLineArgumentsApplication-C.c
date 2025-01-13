#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[], char *envp[])
{
    // variable declarations
    int i;

    // code
    if(argc != 4)
    {
        printf("\n\n");
        printf("Invalid Usgae !!! Exitting Now...\n\n");
        printf("Usage : CommandLineArgumentsApplication <first name> <middle name> <surname>\n\n");
        exit(0);
    }

    // *** THIS PROGRAMS PRINTS YOUR FULL NAME AS ENTERED IN THE COMMAND LINE ARGUMENTS ***
    printf("\n\n");
    printf("Your Full Name Is : ");
    for(i = 1; i < argc; i++)
    {
        printf("%s ", argv[i]);
    }

    printf("\n\n");

    return(0);
}
