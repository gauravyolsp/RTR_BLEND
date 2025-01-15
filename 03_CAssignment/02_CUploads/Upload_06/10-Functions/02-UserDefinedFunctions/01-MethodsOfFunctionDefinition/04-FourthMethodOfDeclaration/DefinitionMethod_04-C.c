#include <stdio.h>

int main(int argc, char *argv[], char *envp[])
{
    // function prototype
    int MyAddition(int, int);

    // variable declarations
    int a, b, result;

    // code
    printf("\n\n");
    printf("Enter Intger Value For 'A' : ");
    scanf("%d", &a);

    printf("\n\n");
    printf("Enter Intger Value For 'B' : ");
    scanf("%d", &b);

    result = MyAddition(a, b); // function call
    
    printf("\n\n");
    printf("Sum of %d AND %d = %d\n\n", a, b, result);
    
    return(0);
}

int MyAddition(int a, int b)
{
    // variable declarations : local variables to MyAddition()
    int sum;

    // code
    sum = a + b;
    return(sum);
}
