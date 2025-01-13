#include <stdio.h>

int main(void)
{
    // function prototypes
    void change_count(void);

    // Variable declarations
    extern int global_count;

    // code
    printf("\n");
    printf("Value Of global_count before Change_count() = %d\n", global_count);
    change_count();
    printf("Value Of global_count after change_count() = %d\n", global_count);
    printf("\n");
    return(0);
}

int global_count = 0;

void change_count(void)
{
    // code
    global_count = 5;
    printf("Value Of Global_count in change_count() = %d\n", global_count);
}
