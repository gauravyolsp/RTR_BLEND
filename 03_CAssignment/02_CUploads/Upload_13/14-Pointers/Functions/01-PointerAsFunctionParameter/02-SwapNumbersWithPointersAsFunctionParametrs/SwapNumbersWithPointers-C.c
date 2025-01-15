#include <stdio.h>

int main(void)
{
    // function declarations
    void SwapNumbers(int *, int *);
    

    // variable declarations
    int a;
    int b;

    // code
    printf("\n\n");
    printf("Enter Value For 'A' : ");
    scanf("%d", &a);

    printf("\n\n");
    printf("Enter Value For 'B' : ");
    scanf("%d", &b);

    printf("\n\n");
    printf("****** BEFORE SWAPPING ******\n\n");
    printf("Value of 'A' = %d\n\n", a);
    printf("Value of 'B' = %d\n\n", b);

    SwapNumbers(&a, &b); // ****** ARGUMENTS PASSES 'BY REFERENCE / ADDRESS' ... ******
    
    printf("\n\n");
    printf("****** AFTER SWAPPING ******\n\n");
    printf("Value of 'A' = %d\n\n", a);
    printf("Value of 'B' = %d\n\n", b);

    return(0);
}

// Address of 'a' is copied into 'x' and address of 'b' is copied into 'y' ... So, '&a' and 'x' are poinitng to ONE and the SAME address and '&b' and 'y' are pointing to ONE and the SAME address ...

// Swapping takes place between 'value at address of x' (value at &a i.e : 'a') and 'value at address of y' (value ata %b i.e : 'b') ...

// Hence, Swapping in this case takes place between '*x' and '*y' AS WELL AS BETWEEN 'a' and 'b'...

void SwapNumbers(int *x, int *y)
{
    // variable declarations
    int temp;

    // code
    printf("\n\n");
    printf("****** BEFORE SWAPPING ******\n\n");
    printf("Value of 'X' = %d\n\n", *x);
    printf("Value of 'Y' = %d\n\n", *y);

    temp = *x;
    *x = *y;
    *y = temp;

    printf("\n\n");
    printf("****** AFTER SWAPPING ******\n\n");
    printf("Value of 'X' = %d\n\n", *x);
    printf("Value of 'Y' = %d\n\n", *y);
}
