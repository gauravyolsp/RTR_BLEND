#include <stdio.h>

#define MY_PI 3.1415926535897932

#define AMC_STRING	"AstroMediComp RTR"

//If First Constant is not Assigned A value, it is assumed To be 0 i.e: 'SUNDAY' will Be 0
// And the rest of the contsants are assigned consecutive Integre Values From 0 onwards i.e: 'MONDAY' will be 1, 'TUESDAY' will be 2, and so on...

// Un-named enums
enum
{
	SUNDAY,
	MONDAY,
	TUESDAY,
	WEDNESDAY,
	THURSDAY,
	FRIDAY,
	SATURDAY
};

enum
{
	JANUARY = 1,
	FEBRUARY,
	MARCH,
	APRIL,
	MAY,
	JUNE,
	JULY,
	AUGUST,
	SEPTEMBER,
	OCTOBER,
	NOVEMBER,
	DECEMBER
};

// Named enums
enum Numbers
{
	ONE,
	TWO,
	THREE,
	FOUR,
	FIVE = 5,
	SIX,
	SEVEN,
	EIGHT,
	NINE,
	TEN
};

enum boolean
{
	TRUE = 1,
	FALSE = 0
};

int main(void)
{
	//local contsant declarations
	const double epsilon = 0.000001;
	
	//code
	printf("\n\n");
	printf("Local Constant Epsilon = %lf\n", epsilon);
	
	printf("Sundaya is daya number = %d\n", SUNDAY);
	printf("Monday is daya number = %d\n", MONDAY);
	printf("Tuesday is daya number = %d\n", TUESDAY);
	printf("Wednesday is daya number = %d\n", WEDNESDAY);
	printf("Thrusday is daya number = %d\n", THURSDAY);
	printf("Friday is daya number = %d\n", FRIDAY);
	printf("Saturday is daya number = %d\n", SATURDAY);
	
	printf("One is Enum number = %d\n", ONE);
	printf("Two is Enum number = %d\n", TWO);
	printf("Three is Enum number = %d\n", THREE);
	printf("Four is Enum number = %d\n", FOUR);
	printf("five is Enum number = %d\n", FIVE);
	printf("six is Enum number = %d\n", SIX);
	printf("Seven is Enum number = %d\n", SEVEN);
	printf("Eight is Enum number = %d\n", EIGHT);
	printf("Nine is Enum number = %d\n", NINE);
	printf("Ten is Enum number = %d\n", TEN);
	
	printf("January is Month Number = %d\n", JANUARY);
	printf("Februray is Month Number = %d\n", FEBRUARY);
	printf("March is Month Number = %d\n", MARCH);
	printf("April is Month Number = %d\n", APRIL);
	printf("May is Month Number = %d\n", MAY);
	printf("June is Month Number = %d\n", JUNE);
	printf("July is Month Number = %d\n", JULY);
	printf("August is Month Number = %d\n", AUGUST);
	printf("September is Month Number = %d\n", SEPTEMBER);
	printf("October is Month Number = %d\n", OCTOBER);
	printf("November is Month Number = %d\n", NOVEMBER);
	printf("December is Month Number = %d\n", DECEMBER);
	
	printf("Value of TRUE is = %d\n", TRUE);
	printf("Value of FALSE is = %d\n", FALSE);
	
	printf("MY_PI Macro value = %.101f\n\n", MY_PI);
	printf("Area of Cirecle of Radius 2 units = %f\n\n", (MY_PI * 2.0f * 2.0f)); //pi * r * r = area of circle of radius'r'
	
	printf("\n\n");
	
	printf(AMC_STRING);
	printf("\n\n");
	
	printf("AMC_STRING is : %s\n", AMC_STRING);
	printf("\n\n");
	
	return(0);
}