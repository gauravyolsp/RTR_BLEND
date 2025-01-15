#include <stdio.h>

int main(void)
{
  // variable declarations
  char chArray_01[] = {'A', 'S', 'T', 'R', 'O', 'M', 'E', 'D', 'I', 'C', 'O', 'M', 'P', '\0'}; // Must give \0 Explicitly for Proper Initialization
  char chArray_02[9] = {'W', 'E', 'L', 'C', 'O', 'M', 'E', 'S', '\0'}; // Must give \0 Explicitly for proper initialization
  char chArray_03[] = {'Y', 'O', 'U', '\0'}; //Must give \0 Explicitly for proper Initialization
  char chArray_04[] = "To"; // \0 is assumed, size is given as 3, although string has only 2 characters
  char chArray_05[] = "REAL TIME RENDERING BATCH OF 2020-21"; // \0 is assumed, size is given as 40, although string has 39 charcaters

  char chArray_WithoutNullTerminator[] = {'H', 'e', 'l', 'l', 'o'};

  // code 
  printf("\n\n");

  printf("Size of chArray_01 : %zu\n\n", sizeof(chArray_01));
  printf("Size of chArray_02 : %zu\n\n", sizeof(chArray_02));
  printf("Size of chArray_03 : %zu\n\n", sizeof(chArray_03));
  printf("Size of chArray_04 : %zu\n\n", sizeof(chArray_04));
  printf("Size of chArray_05 : %zu\n\n", sizeof(chArray_05));

  printf("\n\n");

  printf("The Strings Are : \n\n");
  printf("ChArray_01 : %s\n\n", chArray_01);
  printf("ChArray_02 : %s\n\n", chArray_02);
  printf("ChArray_03 : %s\n\n", chArray_03);
  printf("ChArray_04 : %s\n\n", chArray_04);
  printf("ChArray_05 : %s\n\n", chArray_05);

  printf("\n\n");
  printf("Size of chArray_WithoutNullTerminator : %zu\n\n", sizeof(chArray_WithoutNullTerminator));
  printf("chArray_WithoutNullTerminator : %s\n\n", chArray_WithoutNullTerminator); // Will display garbage value at the end of string due to absence of \0

  return(0);
}
