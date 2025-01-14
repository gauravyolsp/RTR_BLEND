#include <stdio.h>

enum
{
    NEGATIVE = -1,
    ZERO,
    POSITIVE
};

int main(void)
{
    // function declarations
    int Difference(int, int, int *);

    // variable declaration
    int a;
    int b;
    int answer, ret;

    // code
    printf("\n\n");
    printf("Enter Value of 'A' : ");
    scanf("%d", &a);

    printf("\n\n");
    printf("Enter Value of 'B' : ");
    scanf("%d", &b);

    ret = Difference(a, b, &answer);

    printf("\n\n");
    printf("Differemce of %d and %d = %d\n\n", a, b, answer);

    if(ret == POSITIVE)
        printf("The Difference of %d and %d is positive !!!\n\n", a, b);

    else if(ret == NEGATIVE)
        printf("The Difference of %d and %d is negative !!!\n\n", a, b);
    else
        printf("The Difference of %d and %d is ZERO !!!\n\n", a, b);

    return(0);
}

int Difference(int x, int y, int *diff)
{
    // code
    *diff = x - y;

    if(*diff > 0)
        return(POSITIVE);
    else if(*diff < 0)
        return(NEGATIVE);
    else 
        return(ZERO);
}
