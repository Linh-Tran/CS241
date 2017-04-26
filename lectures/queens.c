#include <stdio.h>

#define MAX_SIZE 25

/* Check if position is safe given previously placed queens. */
int safe(int positions[], int row, int col)
{
  int i;
 
  for(i = 0; i < row; ++i)
  {
    if(positions[i] == col ||
       positions[i] == col - (row - i) ||
       positions[i] == col + (row - i))
    {
      return 0;
    }
  }
  return 1;
}

/* Print nice 2D board with queen positions. */
void print_board(int positions[], int n)
{
  int row, col;
  for(row = 0; row < n; ++row)
  {
    for(col = 0; col < n; ++col)
    {
      if(positions[row] == col)	printf("Q ");
      else printf(". ");
    }
    printf("\n");
  }
  printf("\n");
}

void place_queen(int positions[], int n, int row)
{
  int col;

  if(row == n) print_board(positions, n);
  else
  {
    for(col = 0; col < n; ++col)
    {
      if(safe(positions, row, col))
      {
	positions[row] = col;	
	place_queen(positions, n, row+1);
      }
    }
  }
}

/* User can give value of N as command line argument. Defaults to 4. */
int main(int argc, char** argv)
{
  int positions[MAX_SIZE];
  int n = (argc > 1) ? atoi(argv[1]) : 4;
  if(n > MAX_SIZE) printf("I can't handle that many queens!\n");
  else place_queen(positions, n, 0);
  return 0;
}
