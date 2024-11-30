#include <stdio.h>

int main(void)
{
	//code 
	printf("\n\n");
	printf("Going on to Next Line...Using \\n Escape Sequence\n\n");
	printf("Demonstrating \t Horizontal \t Tab \t using \t \\t Escape sequence !!!\n\n");
	printf("\"This is a Double Quoted Output\" Done using \\\" \\\" Escape sequence\n\n");
	printf("\"This is a Single Quoted Output\" Done using \\\' \\\' Escape sequence\n\n");
	printf("BACKSPACE Turned to BACKSPACE\b using escape Sequence \\b\n\n");
	
	printf("\r Demonstarting Carriage return using \\r Escape Sequence\n");
	printf("Demonstarting \r carriage Return using \\r Escape Sequence\n");
	printf("Demonstarting carriage \r return using \\r Escape Sequnce\n\n");
	
	printf("Demonstarting \x41 using \\xhh Escape Sequence\n\n");
	//0x41 is the Hexadecimal code for Letter 'A'. 'xhh' is the place-holder for 'x' Followed by 2 digits (hh), altogether forming a Hexadecimal Number.
	
	printf("Demonstarting \102 using \\ooo Escape Seaquence\n\n"); //102 is the Octal code for letter 'B'. 'ooo' is the place-holder for 3 digits forming an octal number.
	
	return(0);
}
