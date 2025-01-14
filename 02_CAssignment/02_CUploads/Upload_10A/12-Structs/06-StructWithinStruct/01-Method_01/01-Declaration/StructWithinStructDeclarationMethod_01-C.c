#include <stdio.h>

struct Rectangle 
{
    struct MyPoint
    {
        int x;
        int y;
    }point_01, point_02;
}rect;

int main(void)
{
    // variable declartions
    int length, breadth, area;

    // code
    printf("\n\n");
    printf("Enter Left most X-Coordinate Of Rectangle : ");
    scanf("%d", &rect.point_01.x);

    printf("\n\n");
    printf("Enter Bottom most Y-Coordinate of Rectangle : ");
    scanf("%d", &rect.point_01.y);

    printf("\n\n");
    printf("Enter Right most X-Coordinate of Rectangle : ");
    scanf("%d", &rect.point_02.x);

    printf("\n\n");
    printf("Enter Top most Y-Coordinate of Rectangle : ");
    scanf("%d", &rect.point_02.y);

    length = rect.point_02.y - rect.point_01.y;
    if(length < 0)
        length = length * -1;
    
    breadth = rect.point_02.x - rect.point_01.x;
    if(breadth < 0)
        breadth = breadth * -1;

    area = length * breadth;

    printf("\n\n");
    printf("Length of Rectangle = %d\n\n", length);
    printf("Breadth of Rectangle = %d\n\n", breadth);
    printf("Area Of Rectangle = %d\n\n", area);

    return(0);
}
