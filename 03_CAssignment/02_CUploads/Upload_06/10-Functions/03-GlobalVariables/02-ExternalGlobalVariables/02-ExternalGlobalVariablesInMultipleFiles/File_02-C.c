#include <stdio.h>

void change_count_two(void)
{
    // code
    extern int global_count;
    global_count = global_count + 1;
    printf("change_count_two() : Value of global_count in File_02 = %d\n", global_count);
}
