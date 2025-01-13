#include <stdio.h>

int main(int argc, char *argv[], char *envp[])
{
    // function prototype
    void MyAddition(void);

    // code
    MyAddition(); // function call
    return(0);
}

// ****** USER DEFINED FUNCTION : METHOD OF DEFINITION 1 ******
// ****** NO RETURN VALUE, NO PARAMETERS *******

void MyAddition(void)
{
    // variable declarations : local variables to MyAddition()
    int a, b, sum;

    // code
    printf("\n\n");
    printf("Enter Integer Value for 'A' : ");
    scanf("%d", &a);

    printf("\n\n");
    printf("Enter Integer Value for 'B' : ");
    scanf("%d", &b);
    
    sum = a + b;

    printf("\n\n");
    printf("Sum of %d and %d = %d\n\n", a, b, sum);
}
