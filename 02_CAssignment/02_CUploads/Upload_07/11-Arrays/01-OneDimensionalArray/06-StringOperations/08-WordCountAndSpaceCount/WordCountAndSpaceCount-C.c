#include <stdio.h>

#define MAX_STRING_LENGTH 512

int main(void)
{
  // function prototype
  int MyStrlen(char[]);
  void MyStrcpy(char[], char[]);

  // variable declarations
  char chArray[MAX_STRING_LENGTH]; // A Character Array Is A String
  int iStringLength;
  int i;
  int word_count = 0, space_count = 0;

  // code
  // *** STRING INPUT ***
  printf("\n\n");
  printf("Enter A String : \n\n");
  gets_s(chArray, MAX_STRING_LENGTH);

  iStringLength = MyStrlen(chArray);

  for(i = 0; i < iStringLength; i++)
  {
    switch(chArray[i])
    {
      case 32:
        space_count++;
        break;
      default:
        break;
    }
  }

  word_count = space_count + 1;

  // *** STRING OUTPUT ***
  printf("\n\n");
  printf("String Entered By You Is : \n\n");
  printf("%s\n", chArray);

  printf("\n\n");
  printf("Number Of Space In The Input String = %d\n\n", space_count);
  printf("Number Of Words in the Input String = %d\n\n", word_count);

  return(0);
}

int MyStrlen(char str[])
{
  // variable declarations
  int j;
  int string_length = 0;

  // code
  // *** DETERMINING EXACT LENGTH OF THE STRING, BY DETECTING THE FIRST OCCURENCE OF NULL-TERMINATING CHARACTER (\0) ***

  for(j = 0; j < MAX_STRING_LENGTH; j++)
  {
    if(str[j] == '\0')
      break;
    else
      string_length++;
  }

  return(string_length);
}

void MyStrcpy(char str_destination[], char str_source[])
{
  // function prototype
  int MyStrlen(char[]);

  // variable declarations
  int iStringLength = 0;
  int j;

  // code
  iStringLength = MyStrlen(str_source);

  for(j = 0; j < iStringLength; j++)
    str_destination[j] = str_source[j];

  str_destination[j] = '\0';
}
