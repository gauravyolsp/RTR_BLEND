#include <stdio.h>

int main(void)
{
    // variable declarations
    float fArray[10];
    float *ptr_fArray = NULL;
    int i;

    // code
    for(i = 0; i < 10; i++)
        fArray[i] = (i + 1) * 3;

    // *** NAME OF ANY ARRAY IS ITS BASE ADDRESS ***
    // *** HENCE, 'fArray' IS THE BASE ADDRESS OF ARRAY fArray[] OR 'fArray' IS THE ADDRESS OF ELEMENT fArray[0] ***
    // *** ASSIGNING BASE ADDRESS OF ARRAY 'fArray[]' TO INTEGER POINTER 'ptr_fArray'

    ptr_fArray = fArray; // ptr_fArray = &fArray[0]

    printf("\n\n");
    printf("Elements of the float Array : \n\n");

    for(i = 0; i < 10; i++)
        printf("fArray[%d] = %f\n", i, *(ptr_fArray + i));

    printf("\n\n");
    printf("Elements of the float Array : \n\n");
    for(i = 0; i < 10; i++)
        printf("fArray[%d] = %f \t \t Address = %p\n", i, *(ptr_fArray + i), (ptr_fArray + i));

    printf("\n\n");

    return(0);
}
