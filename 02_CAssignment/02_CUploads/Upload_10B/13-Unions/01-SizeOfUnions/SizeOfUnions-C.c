#include <stdio.h>

struct MyStruct
{
    int i;
    float f;
    double d;
    char c;
};

union MyUnion
{
    int i;
    float f;
    double d;
    char c;
};

int main(void)
{
    // variable declarations
    struct MyStruct s;
    union MyUnion u;

    // code
    printf("\n\n");
    printf("Size Of MyStruct = %zu\n", sizeof(s));
    printf("\n\n");
    printf("Size Of MyUnion = %zu\n", sizeof(u));

    return(0);
}
