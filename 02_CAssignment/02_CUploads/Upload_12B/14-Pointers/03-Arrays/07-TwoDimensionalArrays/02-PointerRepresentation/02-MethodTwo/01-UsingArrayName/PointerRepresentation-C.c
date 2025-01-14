#include <stdio.h>

#define NUM_ROWS 5
#define NUM_COLUMNS 3

int main(void)
{
    // variable declarartions
    int iArray[NUM_ROWS][NUM_COLUMNS];
    int i, j;

    // code
    
    for(i = 0; i < NUM_ROWS; i++)
    {
        for(j = 0; j < NUM_COLUMNS; j++)
            *(*(iArray + i) + j) = (i + 1) * (j + 1); // 'iArray[i]' can be treated as 1D Array using Pointers ...
    }

    printf("\n\n");
    printf("2D Integer Array Elements Along with Addresses : \n\n");
    for(i = 0; i < NUM_ROWS; i++)
    {
        for(j = 0; j < NUM_COLUMNS; j++)
        {
            printf("*(*(iArray + %d) + %d) = %d \t \t At Address (*(iArray + %d) + %d) : %p\n", i, j, *(*(iArray + i) + j), i, j, (*(iArray + i) + j));
        }
        printf("\n\n");
    }
    return(0);
}
