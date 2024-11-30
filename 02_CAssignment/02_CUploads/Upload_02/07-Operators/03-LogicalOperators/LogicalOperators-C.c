#include <stdio.h>

int main(void)
{
	//variable declarations
	int a;
	int b;
	int c;
	int result;
	
	//code
	printf("\n\n");
	printf("Enter First Integer : ");
	scanf("%d", &a);
	
	printf("\n\n");
	printf("Enter Second Integer : ");
	scanf("%d", &b);
	
	printf("\n\n");
	printf("Enter Third Integer : ");
	scanf("%d", &c);
	
	printf("\n\n");
	printf("If Answer = 0, It is 'FALSE'.\n");
	printf("If Answer = 1, It is 'TRUE'.\n");
	
	result = (a <= b) && (b != c);
	printf("LOGICAL AND(&&) : Answer is TRUE (1) if and only if both conditions are True. The Answer is FALSE (0), If any one or both conditions are false.\n\n");
	
	printf("A = %d Is Less Than or Equal To B = %d AND B = %d Is NOt EQUAL To C = %d \t Answer = %d\n\n", a, b, b, c, result);
	
	result = (b >= a) || (a == c);
	printf("LOGICAL OR(||) : Answer is FALSE (0) if and only if both conditions are False. The Answer is TRUE (1), If any one or both conditions are True.\n\n");
	
	printf("Either B = %d Is Greater Than or Equal To A = %d OR A = %d Is EQUAL To C = %d \t Answer = %d\n\n", b, a, a, c, result);
	
	result = !a;
	printf("A = %d and using Logical NOT (!) Operator on A gives result = %d\n\n", a, result);
	
	result = !b;
	printf("B = %d and using Logical NOT (!) Operator on B gives result = %d\n\n", b, result);
	
	result = !c;
	printf("C = %d and using Logical NOT (!) Operator on C gives result = %d\n\n", c, result);
	
	result = (!(a <= b) && !(b != c));
	printf("Using Logical NOT (!) on (a <= b) And also on (b != c) And then ANDing then afterwards Gives result = %d\n", result);
	
	printf("\n\n");
	
	result = !((b >= a) || (a == c));
	printf("Using Logical NOT (!) on entire Logical Expression (b >= a) || (a == c) Gives Result = %d\n", result);
	
	printf("\n\n");
	
	return(0);
}