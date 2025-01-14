#include <stdio.h>
#include <stdlib.h>

int main(void)
{   
    void MyAlloc(int **ptr, unsigned int numberOfElements);

    // variable declarations
    int *piArray = NULL;
    unsigned int num_elements;
    int i;

    // code
    printf("\n\n");
    printf("How many elements you want in integer Array ?\n\n");
    scanf("%u", &num_elements);

    printf("\n\n");
    MyAlloc(&piArray, num_elements);

    printf("Enter %u Elements to Fill Up Ypur Integer Array : \n\n", num_elements); 
    for(i = 0; i < num_elements; i++)
        scanf("%d", &piArray[i]);

    printf("\n\n");
    printf("The %u Elements Entered by you in the Integer Array : \n\n", num_elements);
    for(i = 0; i < num_elements; i++)
        printf("%u\n", piArray[i]);

    printf("\n\n");
    if(piArray)
    {
        free(piArray);
        piArray = NULL;
        printf("Memory Allocated has been successfully Freed !!!\n\n");
    }

    return(0);
}

void MyAlloc(int **ptr, unsigned int numberOfElements)
{
    // code
    *ptr = (int *)malloc(numberOfElements * sizeof(int));
    if(*ptr == NULL)
    {
        printf("Could Not Allocate Memory !!! Exiting Now... \n\n");
        exit(0);
    }

    printf("MyAlloc() has successfully Allocated %zu Bytes For Integer Array !!!\n\n", (numberOfElements * sizeof(int)));
}
