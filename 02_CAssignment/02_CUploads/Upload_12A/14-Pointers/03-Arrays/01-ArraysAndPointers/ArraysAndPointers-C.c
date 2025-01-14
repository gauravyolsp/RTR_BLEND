#include <stdio.h>

int main(void)
{
    // variable declarations
    int iArray[] = {12, 24, 36, 48, 60, 72, 84, 96, 108, 120};
    float fArray[] = {9.8f, 8.7f,7.6f, 6.5f, 5.4f};
    double dArray[] = {1.222222, 2.333333, 3.444444};
    char cArray[] = {'A', 'S', 'T', 'R', 'O', 'M', 'E', 'D', 'I', 'C', 'O', 'M', 'P', '\0'};

    // code
    printf("\n\n");
    printf("Integer Array Elements and the addresses they occupy are as Follows : \n\n");
    printf("iArray[0] = %d \t At Address : %p\n", *(iArray + 0), (iArray + 0));
    printf("iArray[1] = %d \t At Address : %p\n", *(iArray + 1), (iArray + 1));
    printf("iArray[2] = %d \t At Address : %p\n", *(iArray + 2), (iArray + 2));
    printf("iArray[3] = %d \t At Address : %p\n", *(iArray + 3), (iArray + 3));
    printf("iArray[4] = %d \t At Address : %p\n", *(iArray + 4), (iArray + 4));
    printf("iArray[5] = %d \t At Address : %p\n", *(iArray + 5), (iArray + 5));
    printf("iArray[6] = %d \t At Address : %p\n", *(iArray + 6), (iArray + 6));
    printf("iArray[7] = %d \t At Address : %p\n", *(iArray + 7), (iArray + 7));
    printf("iArray[8] = %d \t At Address : %p\n", *(iArray + 8), (iArray + 8));
    printf("iArray[9] = %d \t At Address : %p\n", *(iArray + 9), (iArray + 9));
}
