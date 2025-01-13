#include <stdio.h>

#define MAX_STRING_LENGTH 512

int main(void)
{
  // variable declarations
  char chArray[MAX_STRING_LENGTH]; // A Character Array is a string
  int iStringLength = 0;

  // code

  // *** STRING INPUT ***
  printf("\n\n");
  printf("Enter A String : \n\n");
  gets_s(chArray, MAX_STRING_LENGTH);

  // *** STRING OUTPUT ***
  printf("\n\n");
  printf("String Entered By You Is : \n\n");
  printf("%s\n", chArray);

  // *** STRING LENGTH ***
  printf("\n\n");
  iStringLength = strlen(chArray);
  printf("Length of string is = %d Characters !!!\n\n", iStringLength);

  return(0);
}
