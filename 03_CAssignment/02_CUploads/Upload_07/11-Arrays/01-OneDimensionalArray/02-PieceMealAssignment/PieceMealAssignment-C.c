#include <stdio.h>

int main(void)
{
  // variable declarations
  int iArrayOne[10];
  int iArrayTwo[10];

  //code
  // ****** iArrayOne[] ******
  iArrayOne[0] = 3;
  iArrayOne[1] = 6;
  iArrayOne[2] = 9;
  iArrayOne[3] = 12;
  iArrayOne[4] = 15;
  iArrayOne[5] = 18;
  iArrayOne[6] = 21;
  iArrayOne[7] = 24;
  iArrayOne[8] = 27;
  iArrayOne[9] = 30;
  
  printf("\n\n");
  printf("Piece-meal (Hard-coded) Assignment And Display of elements to array 'iArrayOne[]': \n\n");
  
  printf("1st Element of Array 'iArrayOne[]' or Element at 0th Index of Array 'iArrayOne[]' = %d\n", iArrayOne[0]);
  printf("2nd Element of Array 'iArrayOne[]' or Element at 1st Index of Array 'iArrayOne[]' = %d\n", iArrayOne[1]);
  printf("3rd Element of Array 'iArrayOne[]' or Element at 2nd Index of Array 'iArrayOne[]' = %d\n", iArrayOne[2]);
  printf("4th Element of Array 'iArrayOne[]' or Element at 3rd Index of Array 'iArrayOne[]' = %d\n", iArrayOne[3]);
  printf("5th Element of Array 'iArrayOne[]' or Element at 4th Index of Array 'iArrayOne[]' = %d\n", iArrayOne[4]);
  printf("6th Element of Array 'iArrayOne[]' or Element at 5th Index of Array 'iArrayOne[]' = %d\n", iArrayOne[5]);
  printf("7th Element of Array 'iArrayOne[]' or Element at 6th Index of Array 'iArrayOne[]' = %d\n", iArrayOne[6]);
  printf("8th Element of Array 'iArrayOne[]' or Element at 7th Index of Array 'iArrayOne[]' = %d\n", iArrayOne[7]);
  printf("9th Element of Array 'iArrayOne[]' or Element at 8th Index of Array 'iArrayOne[]' = %d\n", iArrayOne[8]);
  printf("10th Element of Array 'iArrayOne[]' or Element at 9th Index of Array 'iArrayOne[]' = %d\n", iArrayOne[9]);

  // ****** iArrayTwo[] ******
  printf("\n\n");

  printf("Enter 1st Element of Array 'iArrayTwo[]' : ");
  scanf("%d", &iArrayTwo[0]);
  printf("Enter 2nd Element of Array 'iArrayTwo[]' : ");
  scanf("%d", &iArrayTwo[1]);
  printf("Enter 3rd Element of Array 'iArrayTwo[]' : ");
  scanf("%d", &iArrayTwo[2]);
  printf("Enter 4th Element of Array 'iArrayTwo[]' : ");
  scanf("%d", &iArrayTwo[3]);
  printf("Enter 5th Element of Array 'iArrayTwo[]' : ");
  scanf("%d", &iArrayTwo[4]);
  printf("Enter 6th Element of Array 'iArrayTwo[]' : ");
  scanf("%d", &iArrayTwo[5]);
  printf("Enter 7th Element of Array 'iArrayTwo[]' : ");
  scanf("%d", &iArrayTwo[6]);
  printf("Enter 8th Element of Array 'iArrayTwo[]' : ");
  scanf("%d", &iArrayTwo[7]);
  printf("Enter 9th Element of Array 'iArrayTwo[]' : ");
  scanf("%d", &iArrayTwo[8]);
  printf("Enter 10th Element of Array 'iArrayTwo[]' : ");
  scanf("%d", &iArrayTwo[9]);

  printf("\n\n");
  printf("Piece-meal (Hard-coded) Assignment And Display of elements to array 'iArrayTwo[]': \n\n");
  printf("1st Element of Array 'iArrayTwo[]' or Element at 0th Index of Array 'iArrayTwo[]' = %d\n", iArrayTwo[0]);
  printf("2nd Element of Array 'iArrayTwo[]' or Element at 1st Index of Array 'iArrayTwo[]' = %d\n", iArrayTwo[1]);
  printf("3rd Element of Array 'iArrayTwo[]' or Element at 2nd Index of Array 'iArrayTwo[]' = %d\n", iArrayTwo[2]);
  printf("4th Element of Array 'iArrayTwo[]' or Element at 3rd Index of Array 'iArrayTwo[]' = %d\n", iArrayTwo[3]);
  printf("5th Element of Array 'iArrayTwo[]' or Element at 4th Index of Array 'iArrayTwo[]' = %d\n", iArrayTwo[4]);
  printf("6th Element of Array 'iArrayTwo[]' or Element at 5th Index of Array 'iArrayTwo[]' = %d\n", iArrayTwo[5]);
  printf("7th Element of Array 'iArrayTwo[]' or Element at 6th Index of Array 'iArrayTwo[]' = %d\n", iArrayTwo[6]);
  printf("8th Element of Array 'iArrayTwo[]' or Element at 7th Index of Array 'iArrayTwo[]' = %d\n", iArrayTwo[7]);
  printf("9th Element of Array 'iArrayTwo[]' or Element at 8th Index of Array 'iArrayTwo[]' = %d\n", iArrayTwo[8]);
  printf("10th Element of Array 'iArrayTwo[]' or Element at 9th Index of Array 'iArrayTwo[]' = %d\n", iArrayTwo[9]);

  return(0);
}
