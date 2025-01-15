#include <stdio.h>
#include <conio.h>

int main(void)
{
	//variable declarations
	int i;
	char ch;
	
	//code
	printf("\n\n");
	
	printf("Printing Even Numbers from 1 to 100 for Every user Input. Exiting the Loop when user Enters Character 'Q' or 'q' : \n\n");
	printf("Enter Character 'Q' or 'q' To Exit Loop : \n\n");
	
	for(i = 1; i <= 100; i++)
	{
		printf("\t%d\n", i);
		ch = getch();
		
		if(ch == 'Q' || ch == 'q')
		{
			break;
		}
	}
	
	printf("\n\n");
	printf("EXITING LOOP...");
	printf("\n\n");

	return(0);
}