#include <stdio.h>

int main(int argc, char *argv[], char *envp[])
{
    // function prototypes 
    void MyAddition(void);
    int MySubtraction(void);
    void MyMultiplication(int, int);
    int MyDivision(int ,int);

    // variable declarations
    int result_subtraction;
    int a_multiplication, b_multiplication;
    int a_division, b_division, result_division;

    // code
    // *** ADDITION ***
    MyAddition(); // function call

    // *** SUBTRACTION ***
    result_subtraction = MySubtraction();
    printf("\n\n");
    printf("Subtraction Yields Result = %d\n", result_subtraction);

    // *** MULTPLICATION ***
    printf("\n\n");
    printf("Enter Integer Value For 'A' For Multiplication : ");
    scanf("%d", &a_multiplication);

    printf("\n\n");
    printf("Enter Integer Value For 'B' For Multiplication : ");
    scanf("%d", &b_multiplication);

    MyMultiplication(a_multiplication, b_multiplication);

    // *** DIVISION ***
    printf("\n\n");
    printf("Enter Integer Value For 'A' For Division : ");
    scanf("%d", &a_division);

    printf("\n\n");
    printf("Enter Integer Value For 'B' For Division : ");
    scanf("%d", &b_division);

    result_division = MyDivision(a_division, b_division);
    printf("\n\n");
    printf("Division of %d and %d Gives = %d (Quotient)\n", a_division, b_division, result_division);

    printf("\n\n");

    return(0);
}

// *** Function Definition Of MyAddition ******
void MyAddition(void) // Function definition
{
    // variable declarations
    int a, b, sum;

    // code
    printf("\n\n");
    printf("Enter Integer Value For 'A' For Addition : ");
    scanf("%d", &a);

    printf("\n\n");
    printf("Enter Integer Value For 'B' For Addition : ");
    scanf("%d", &b);

    sum = a + b;

    printf("\n\n");
    printf("Sum of %d and %d = %d\n\n", a, b, sum);
}

// ** Function Definition of MySubtraction() ******
int MySubtraction(void)
{
    // variable declarations
    int a, b, subtraction;

    // code
    printf("\n\n");
    printf("Enter Integer Value For 'A' For Subtraction : ");
    scanf("%d", &a);

    printf("\n\n");
    printf("Enter Integer Value For 'B' For Subtraction : ");
    scanf("%d", &b);

    subtraction = a - b;
    return(subtraction);
}

// *** Function Definition of MyMultiplication() *****
void MyMultiplication(int a, int b)
{
    // variable declarations
    int multiplication;

    // code
    multiplication = a * b;

    printf("\n\n");
    printf("Multiplication OF %d And %d = %d\n\n", a, b, multiplication);
}

// *** Function Definition Of MyDivision() ******
int MyDivision(int a, int b)
{
    // variable declarations 
    int divison_quotient;

    // code
    if(a > b)
        divison_quotient = a / b;
    else
        divison_quotient = b / a;

    return(0);
}
