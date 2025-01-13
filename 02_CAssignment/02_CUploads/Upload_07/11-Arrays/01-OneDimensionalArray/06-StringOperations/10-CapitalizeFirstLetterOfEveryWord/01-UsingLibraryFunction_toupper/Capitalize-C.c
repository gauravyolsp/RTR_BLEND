#include <stdio.h>
#include <ctype.h>

#define MAX_STRING_LENGTH 512

int main(void)
{
    // function prototype
    int MyStrlen(char[]);

    // variable delcarations
    char chArray[MAX_STRING_LENGTH], chArray_CapitalizedFirstLetterOfEveryWord[MAX_STRING_LENGTH];

    int iStringLength;
    int i, j;

    // code
    // *** STRING INPUT ***
    printf("\n\n");
    printf("Enter A string : \n\n");
    gets_s(chArray, MAX_STRING_LENGTH);
    
    iStringLength = MyStrlen(chArray);
    j = 0;
    for(i = 0; i < iStringLength; i++)
    {
        if(i == 0)
            chArray_CapitalizedFirstLetterOfEveryWord[j] = toupper(chArray[i]);
        else if(chArray[i] == ' ')
        {
            chArray_CapitalizedFirstLetterOfEveryWord[j] = chArray[i];
            chArray_CapitalizedFirstLetterOfEveryWord[j + 1] = toupper(chArray[i + 1]);

            j++;
            i++;
        }
        else
            chArray_CapitalizedFirstLetterOfEveryWord[j] = chArray[i];

        j++;
    }

    chArray_CapitalizedFirstLetterOfEveryWord[j] = '0';

    // *** STRING OUTPUT ***
    printf("\n\n");
    printf("String Entered By You Is : \n\n");
    printf("%s\n", chArray);

    printf("\n\n");
    printf("String After Capitalizing First Letter Of Every Word : \n\n");
    printf("%s\n", chArray_CapitalizedFirstLetterOfEveryWord);

    return(0);
}

int MyStrlen(char str[])
{
    // variable declarations
    int j;
    int string_legth = 0;

    // code
    for(j = 0; j < MAX_STRING_LENGTH; j++)
    {
        if(str[j] == '\0')
            break;
        else
            string_legth++;
    }
    return(string_legth);
}
