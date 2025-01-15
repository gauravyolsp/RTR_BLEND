#include <stdio.h>

// MACRO CONSTANT USED AS ARRAY SIZE IN SUBSCRIPT AND AS ARRAY LENGTH.
// HENCE, THIS PROGRAM'S ARRAYS SIZES CAN BE SIMPLY CHANGED BY CHANGING THESE FOLLOWING 3 GLOBAL MACRO CONSTANT VALUES, BEFORE COMPILING, LINKING AND EXECUTING THE PROGRAM !!!

#define INT_ARRAY_NUM_ELEMENTS 5
#define FLOAT_ARRAY_NUM_ELEMENTS 3
#define CHAR_ARRAY_NUM_ELEMENTS 15

int main(void)
{
  // variable declarations
  int iArray[INT_ARRAY_NUM_ELEMENTS];
  float fArray[FLOAT_ARRAY_NUM_ELEMENTS];
  char cArray[CHAR_ARRAY_NUM_ELEMENTS];
  int i, num;

  // code

  // ********** ARRAY ELEMENTS INPUT **********
  printf("\n\n");
  printf("Enter Elements for 'Interger' Array : \n");
  for(i = 0; i < INT_ARRAY_NUM_ELEMENTS; i++)
    scanf("%d", &iArray[i]);

  printf("\n\n");
  printf("Enter Elements for 'Floating-Point' Array : \n");
  for(i = 0; i < FLOAT_ARRAY_NUM_ELEMENTS; i++)
    scanf("%f", &fArray[i]);

  printf("\n\n");
  printf("Enter Elements for 'Character' Array : \n");
  for(i = 0; i < CHAR_ARRAY_NUM_ELEMENTS; i++)
  {
    cArray[i] = getch();
    printf("%c\n", cArray[i]);
  }

  // ******** ARRAY ELEMENTS OUTPUT **********
  printf("\n\n");
  printf("Integer Array Entered By you : \n\n");
  for(i = 0; i < INT_ARRAY_NUM_ELEMENTS; i++)
    printf("%d\n", iArray[i]);

  printf("\n\n");
  printf("Floating-Point Array Entered By you : \n\n");
  for(i = 0; i < FLOAT_ARRAY_NUM_ELEMENTS; i++)
    printf("%f\n", fArray[i]);

  printf("\n\n");
  printf("Character Array Entered By you : \n\n");
  for(i = 0; i < CHAR_ARRAY_NUM_ELEMENTS; i++)
    printf("%c\n", cArray[i]);

  return(0);
}
