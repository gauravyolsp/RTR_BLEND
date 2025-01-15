#include <stdio.h>
#include <stdlib.h>

// DEFINIGN STRUCT
struct MyData
{
    int i;
    float f;
    double d;
};

int main(void)
{
    // function prototype
    void ChangeValues(struct MyData *);

    // variable declarations
    struct MyData *pData = NULL;

    // code
    printf("\n\n");

    pData = (struct MyData *)malloc(sizeof(struct MyData));
    if(pData == NULL)
    {
        printf("FAILED TO ALLOCATE MEMORY TO 'struct MyData' !!! EXITING NOW\n\n");
        exit(0);
    }
    else
        printf("SUCCESSFULLY ALLOCATED MEMORY TO 'sy=triuct MyData' !!!\n\n");

    // Assigning Data values To the Data Members OF 'struct MyData'
    pData->i = 30;
    pData->f = 11.45f;
    pData->d = 1.2995;

    // Displaying Values Of The Data Members of 'struct MyData'
    printf("\n\n");
    printf("DATA MEMBERS OF 'struct MyData' ARE : \n\n");
    printf("i = %d\n", pData->i);
    printf("f = %f\n", pData->f);
    printf("d = %lf\n", pData->d);

    ChangeValues(pData);

    // Displaying Values Of The Data Members of 'struct MyData'
    printf("\n\n");
    printf("DATA MEMBERS OF 'struct MyData' ARE : \n\n");
    printf("i = %d\n", pData->i);
    printf("f = %f\n", pData->f);
    printf("d = %lf\n", pData->d);

    if(pData)
    {
        free(pData);
        pData = NULL;
        printf("MEMORY ALLOCATED TO 'struct MyData' HAS BEEN SUCCESSFULLY FREED !!!\n\n");
    }

    return(0);
}

void ChangeValues(struct MyData *pParam_Data)
{
    // code
    pParam_Data->i = 9;
    pParam_Data->f = 8.2f;
    pParam_Data->d = 6.1998;

    // CAN ALSO DO THIS AS ...
    /*
        (*pParam_Data).i = 9;
        (*pParam_Data).f = 8.2f;
        (*pParam_Data).d = 6.1998;
    */
}
