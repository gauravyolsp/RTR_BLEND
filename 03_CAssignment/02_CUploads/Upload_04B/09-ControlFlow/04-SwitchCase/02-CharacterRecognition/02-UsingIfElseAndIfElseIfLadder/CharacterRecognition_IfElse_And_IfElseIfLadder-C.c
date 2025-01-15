#include <stdio.h> //for printf()
#include <conio.h> //for getch()

//ASCII Values For 'A' to 'Z' => 65 to 90
#define CHAR_ALPHABET_UPPER_CASE_BEGINNING 	65
#define CHAR_ALPHABET_UPPER_CASE_ENDING		90

//ASCII Values for 'a' to 'z' => 97 to 122
#define CHAR_ALPHABET_LOWER_CASE_BEGINNING 	97
#define CHAR_ALPHABET_LOWER_CASE_ENDING		122

//ASCII Values for '0' to '9' => 48 to 57
#define CHAR_DIGIT_BEGINNING 	48
#define CHAR_DIGIT_ENDING		57

int main(void)
{
	//varaible declarations
	char ch;
	int ch_value;
	
	//code 
	printf("\n\n");
	
	printf("Enter Character : ");
	ch = getch();
	
	printf("\n\n");
	
	if((ch == 'A' || ch == 'a') || (ch == 'E' || ch == 'e') || (ch == 'I' || ch == 'i') || (ch == 'O' || ch == 'o') || (ch == 'U' || ch == 'u'))
	{
		printf("Character \'%c\' Entered by you, is a VOWEL CHARACTER from the English Alphabet !!!\n\n", ch);
	}
	
	else
	{
		ch_value = (int)ch;
		
		//If the character has ASCII value between 65 AND 90 OR between 97 AND 122, It is still A Letter of the alphabet, But it is A 'CONSONANT', and NOT a 'VOWEL'...
			
		if((ch_value >= CHAR_ALPHABET_UPPER_CASE_BEGINNING && ch_value <= CHAR_ALPHABET_UPPER_CASE_ENDING) || (ch_value >= CHAR_ALPHABET_LOWER_CASE_BEGINNING && ch_value <= CHAR_ALPHABET_LOWER_CASE_ENDING))
		{
			printf("Character \'%c\' Enter by you, Is a CONSONANT CHARACTER from the English Alphabet !!!\n\n", ch);
		}
		
		else if(ch_value >= CHAR_DIGIT_BEGINNING && ch_value <= CHAR_DIGIT_ENDING)
		{
			printf("Character \'%c\' Entered by you, is a DIGIT CHARACTER !!!\n\n", ch);
		}
		
		else
		{
			printf("Character \'%c\' Entered By you, Is a SPECIAL CHARACTER !!!\n\n", ch);
		}
	}
		
	return(0);
}