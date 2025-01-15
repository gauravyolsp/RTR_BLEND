#include <stdio.h>

int main(void)
{
    // DEFINING STRUCT
    struct MyPoint
    {
        int x;
        int y;

    }point; // declaraing a single variable of type 'struct MyPoint' locally ...
    
    // DEFINING STRUCT
    struct MyPointProperties
    {
        int quadrant;
        char axis_location[10];
    }point_properties; // declaraing a single variable of type 'struct MyPointProperties' locally...

    // code
    // User input for the Data Members of 'struct MyPoint' variable 'point_A'
    printf("\n\n");
    printf("Enter X-Coordinate For A Point : ");
    scanf("%d", &point.x);
    printf("Enter Y-Coordinate For A Point : ");
    scanf("%d", &point.y);

    printf("\n\n");
    printf("Point Co-ordinates (x, y) Are : (%d, %d) !!!\n\n", point.x, point.y);

    if(point.x == 0 && point.y == 0)
        printf("The Point is the origin (%d, %d) !!!\n", point.x, point.y);
    else // Atleast One of the two values (either 'X' or 'Y' or 'BOTH) is a non-zero value...
    {
        if(point.x == 0) // If 'X' is zero... Obviously 'Y' is the Non-zero Value 
        {
            if(point.y < 0) // If 'Y' is -ve
                strcpy(point_properties.axis_location, "Negative Y");
            
            if(point.y > 0)
                strcpy(point_properties.axis_location, "Positive Y");
            
            point_properties.quadrant = 0; // A Point Lying On any of the CO-ordinate Axes Is NOT a Part Of ANY Quadrant...

            printf("The Point Lies On The %s Axis !!!\n\n", point_properties.axis_location);
        }
        else if(point.y == 0) // If 'Y' is zero... Obviously 'X' Is the NON_ZERO VALUE
        {
            if(point.x < 0) // If 'X' is -ve
                strcpy(point_properties.axis_location, "Negative X");
            if(point.x > 0) // If 'X' Is +ve
                strcpy(point_properties.axis_location, "Positive X");

            point_properties.quadrant = 0; // A Point Lying On Any oF the Co-ordinte Axes Is Not a Part of any quadrant...
            printf("The Point Lies On The %s Axis !!!\n\n", point_properties.axis_location);
        }
        else // BOTH 'X' AND 'Y' ARE NON-ZERO
        {
            point_properties.axis_location[0] = '\0'; // A Point Lying In Any of the 4 Quadrants Cannot be lying on Any of the Co-ordinate Axes...

            if(point.x > 0 && point.y > 0) // 'X' Is +ve AND 'Y' IS +ve
                point_properties.quadrant = 1;
            else if(point.x < 0 && point.y > 0) // 'X' Is -ve AND 'Y' is +ve
                point_properties.quadrant = 2;
            else if(point.x < 0 && point.y < 0) // 'X' Is -ve AND 'Y' is -ve
                point_properties.quadrant = 3;
            else                                // 'X' Is +ve AND 'Y' Is -ve
                point_properties.quadrant = 4;

            printf("The Point Lies In Quadrant Number %d !!!\n\n", point_properties.quadrant);      
        }
    }

    return(0);
}   
