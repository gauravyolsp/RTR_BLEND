#include <stdio.h>

int main(void)
{
	//variable declaractions
	char option, ch = '\0';
	
	//code
	printf("\n\n");
	printf("Once the Infinite Loop Begins, enter 'Q' or 'q' To Quit the Infinite for loop : \n\n");
	
	do
	{
		do
		{
			printf("\n");
			printf("In Loop...\n");
			ch = getch(); //control flow waits for character input...
		}while(ch == 'Q' || ch == 'q');
		
		printf("\n\n");
		printf("EXITING USER CONTROLLED INFINITE LOOP...");
		
		printf("\n\n");
		printf("DO YOU WANT TO BEGIN USER CONTROLLED INFINITE LOOP AGAIN?... (Y/y - Yes, Any Other Key - No) : ");
		option = getch();
	}while(option == 'Y' || option == 'y');
	
	return(0);
}