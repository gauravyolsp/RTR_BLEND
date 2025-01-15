#include <stdio.h>

int main(int argc, char *argv[], char *envp[])
{
    // function prototype
    int MyAddition(void);

    // variable declarations
    int result;

    // code
    result = MyAddition(); // function call
    
    printf("\n\n");
    printf("Sum = %d\n\n", result);
    return(0);
}

int MyAddition(void)
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

    return(sum);
}
