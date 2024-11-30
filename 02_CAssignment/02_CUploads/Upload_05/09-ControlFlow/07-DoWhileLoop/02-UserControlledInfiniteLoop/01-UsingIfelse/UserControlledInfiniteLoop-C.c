#include <stdio.h>

int main(void)
{
	//variable declaractions
	char option, ch = '\0';
	
	//code
	printf("\n\n");
	printf("Once the Infinite Loop Begins, enter 'Q' or 'q' To Quit the Infinite for loop : \n\n");
	printf("Enter 'Y' or 'y' to Initiate user Controlled Infinite Loop : ");
	printf("\n\n");
	option = getch();
	if(option == 'Y' || option == 'y')
	{
		do
		{
			printf("In Loop...\n");
			ch = getch(); //control flow waits for character input...
			if(ch == 'Q' || ch == 'q')
				break; //User Controlled Exitting from Infinite Loop
		}while(1); //Infinite Loop
		
		printf("\n\n");
		printf("EXITING USER CONTROLLED INFINITE LOOP...");
		printf("\n\n");
	}
	else
	{
		printf("You must press 'Y' or 'y' To initiate the user Controlled Infinite Loop... please try again...\n\n");
	}
	
	return(0);
}