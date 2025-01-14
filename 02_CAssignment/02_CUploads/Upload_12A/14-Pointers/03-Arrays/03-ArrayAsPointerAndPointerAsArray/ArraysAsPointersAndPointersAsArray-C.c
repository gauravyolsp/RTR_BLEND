#include <stdio.h>

int main(void)
{
    // variable declarations
    int iArray[] = {10, 20, 30, 40, 50, 60, 70, 80, 90, 100};
    int *ptr_iArray = NULL;

    // code

    // #### USING ARRAY NAME AS A POINTER i.e : Vlaue of xth Element of iArray : *(iArray + x) AND Address of xth element of iArray : (iArray + x) ####

    printf("\n\n");
    printf("*** USING ARRAY NAME AS A POINTER i.e : Value of xth Element of iArray : *(iArray + x) AND address of xth Element of iArray : (iArray + x) ***\n\n");

    printf("Integer Array Elements and their address : \n\n");
    printf("*(iArray + 0) = %d \t At address (iArray + 0) : %p\n", *(iArray + 0), (iArray + 0));

    printf("*(iArray + 1) = %d \t At address (iArray + 1) : %p\n", *(iArray + 1), (iArray + 1));

    printf("*(iArray + 2) = %d \t At address (iArray + 2) : %p\n", *(iArray + 2), (iArray + 2));

    printf("*(iArray + 3) = %d \t At address (iArray + 3) : %p\n", *(iArray + 3), (iArray + 3));

    printf("*(iArray + 4) = %d \t At address (iArray + 4) : %p\n", *(iArray + 4), (iArray + 4));

    printf("*(iArray + 5) = %d \t At address (iArray + 5) : %p\n", *(iArray + 5), (iArray + 5));

    printf("*(iArray + 6) = %d \t At address (iArray + 6) : %p\n", *(iArray + 6), (iArray + 6));

    printf("*(iArray + 7) = %d \t At address (iArray + 7) : %p\n", *(iArray + 7), (iArray + 7));

    printf("*(iArray + 8) = %d \t At address (iArray + 8) : %p\n", *(iArray + 8), (iArray + 8));

    printf("*(iArray + 9) = %d \t At address (iArray + 9) : %p\n", *(iArray + 9), (iArray + 9));

    // ASSIGING BASE ADDRESS OF INTEGER ARRAY 'iArray' TO INTGER POINTER 'ptr_iArray'
    // NAME OF ANY ARRAY IS ITS OWN BASE ADDERESS
    ptr_iArray = iArray; // SANE AS ...  ptr_iArray = &iArray[0]

    // #### USING POINTER AS ARRAY NAME i.e : Value of xth Element of iArray : ptr_iArray[x] AND Address of xth  Element of iArray : &ptr_iArray[x] ####
    printf("\n\n");
    printf("*** USING POINTER AS ARRAY NAME i.e : Value of xth Element of iArray : ptr_iArray[x] AND Address of xth Element of iarray : &ptr_iArray[x] ***");
    printf("Intger Array Elements And Their Addresses : \n\n");
    printf("ptr_iArray Elements and their Address : \n\n");

    printf("ptr_iArray[0] = %d \t At address &ptr_iArray[0] : %p\n", ptr_iArray[0], &ptr_iArray[0]);

    printf("ptr_iArray[1] = %d \t At address &ptr_iArray[1] : %p\n", ptr_iArray[1], &ptr_iArray[1]);

    printf("ptr_iArray[2] = %d \t At address &ptr_iArray[2] : %p\n", ptr_iArray[2], &ptr_iArray[2]);

    printf("ptr_iArray[3] = %d \t At address &ptr_iArray[3] : %p\n", ptr_iArray[3], &ptr_iArray[3]);

    printf("ptr_iArray[4] = %d \t At address &ptr_iArray[4] : %p\n", ptr_iArray[4], &ptr_iArray[4]);

    printf("ptr_iArray[5] = %d \t At address &ptr_iArray[5] : %p\n", ptr_iArray[5], &ptr_iArray[5]);

    printf("ptr_iArray[6] = %d \t At address &ptr_iArray[6] : %p\n", ptr_iArray[6], &ptr_iArray[6]);

    printf("ptr_iArray[7] = %d \t At address &ptr_iArray[7] : %p\n", ptr_iArray[7], &ptr_iArray[7]);

    printf("ptr_iArray[8] = %d \t At address &ptr_iArray[8] : %p\n", ptr_iArray[8], &ptr_iArray[8]);

    printf("ptr_iArray[9] = %d \t At address &ptr_iArray[9] : %p\n", ptr_iArray[9], &ptr_iArray[9]);

    return(0);
}
