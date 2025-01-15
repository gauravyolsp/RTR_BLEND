#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

#define NAME_LENGTH 100
#define MARITAL_STATUS 10

struct Employee
{
    char name[NAME_LENGTH];
    int age;
    char sex;
    float salary;
    char marital_status;
};

int main(void)
{
    // function prototype
    void MyGetString(char[], int);

    // variable declarations
    struct Employee *pEmployeeRecord = NULL;
    int num_employee, i;

    // code
    printf("\n\n");
    printf("Enter Number Of Employees Whoes Details You Want to Record : ");
    scanf("%d", &num_employee);

    printf("\n\n");
    pEmployeeRecord = (struct Employee *)malloc(sizeof(struct Employee) * num_employee);

    if(pEmployeeRecord == NULL)
    {
        printf("FAILED TO ALLOCATED MEMORY FOR %D EMPLOYEE !!! EXITING NOW...\n\n", num_employee);
        exit(0);
    }
    else
        printf("SUCCESSFULLY ALLOCATED MEMORY FOR %d EMPLOYEES !!!\n\n", num_employee);

    // ****** USER INPUT INITIALIZATION OF ARRAY OF 'struct Employee' ******

    for(i = 0; i < num_employee; i++)
    {
        printf("\n\n\n\n");
        printf("******** DATA ENTRY FOR EMPLOYEE NUMBER %d ********\n", (i + 1));

        printf("\n\n");
        printf("Enter Employee Name : ");
        MyGetString(pEmployeeRecord[i].name, NAME_LENGTH);

        printf("\n\n\n");
        printf("Enter Employee's Age (in years) : ");
        scanf("%d", &pEmployeeRecord[i].age);

        printf("\n\n");
        printf("Enter Employee's sex (M/m For Male, F/f For Female) : ");
        pEmployeeRecord[i].sex = getch();
        printf("%c", pEmployeeRecord[i].sex);
        pEmployeeRecord[i].sex = toupper(pEmployeeRecord[i].sex);

        printf("\n\n\n");
        printf("Enter Emplyee's Salary (in Indian Rupees) : ");
        scanf("%f", &pEmployeeRecord[i].salary);

        printf("\n\n");
        printf("Is The Employee Married? (Y/y For yes, N/n For No) : ");
        pEmployeeRecord[i].marital_status = getch();
        printf("%c", pEmployeeRecord[i].marital_status);
        pEmployeeRecord[i].marital_status = toupper(pEmployeeRecord[i].marital_status);
    }

    // *** DISPLAY ***
    printf("\n\n\n\n");
    printf("******** DISPLAYING EMPLOYEE RECORDS ********\n\n");
    for(i = 0; i < num_employee; i++)
    {
        printf("******** EMPLOYEE NUMBER %d ********\n\n", (1 + 1));
        printf("Name        : %s\n", pEmployeeRecord[i].name);
        printf("Age         : %d years\n", pEmployeeRecord[i].age);

        if(pEmployeeRecord[i].sex == 'M')
            printf("Sex     : MALE\n");
        else if(pEmployeeRecord[i].sex == 'F')
            printf("Sex     : FEMALE\n");
        else
            printf("Sex     : Invalid Data Entered\n");

        printf("Salary      : Rs. %f\n", pEmployeeRecord[i].salary);

        if(pEmployeeRecord[i].marital_status == 'Y')
            printf("Marital Status : Married\n");
        if(pEmployeeRecord[i].marital_status == 'N')
            printf("Marital Status : Unmarried\n");
        else
            printf("Marital Status : Invalid Data Entered\n");
        
        printf("\n\n");
    }

    if(pEmployeeRecord)
    {
        free(pEmployeeRecord);
        pEmployeeRecord = NULL;
        printf("MEMORY ALLOCATED TO %d EMPLOYEES HAS BEEN SUCCESSFULLY FREED !!!\n\n", num_employee);
    }

    return(0);
}

// *** SIMPLE RUDIMENTARY IMPLEMENTATION OF gets_s() ***
// *** IMPLEMENTED DUE TO DIFFERENT BEHAVIOUR OF gets_s() / fgets() / fscanf() ON DIFFERENT PLATFORMS ***
// *** BACKSPACE / CHARACTER DELETION AND ARRAOW KEY CURSOR MOVEMENT NOT IMPLEMENTED ***

void MyGetString(char str[], int str_size)
{
    // variable declarations
    int i;
    char ch = '\0';

    // code
    i = 0;
    do 
    {
        ch = getch();
        str[i] = ch;
        printf("%c", str[i]);
        i++;
    }while((ch != '\r') && (i < str_size));

    if(i == str_size)
        str[i - 1] = '\0';
    else
        str[i] = '\0';
}
