#include <stdio.h>

// *** GLOBAL SCOPE ***
int global_count = 0;

int main(void)
{
    // function prototypes
    void change_count(void);
    void change_count_one(void); // function defines in file_01.c
    void change_count_two(void); // function defines in file_02.c

    // code
    printf("\n");

    change_count();
    change_count_one();
    change_count_two();

    return(0);
}

void change_count(void)
{
    // code
    global_count = global_count + 1;
    printf("Global Count = %d\n", global_count);
}
