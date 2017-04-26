/****************************************************************************
* Linh Tran                                                                
* Date: September 29, 2014                                                 
* CS-241 Section # 1                                                       
* Sudoku Solver
* This program reads a list of numbers that represents a Sudoku puzzle.
* It solves the puzzle and prints out "Error" when a puzzle input is invalid,
* contain duplicates, and unsolvable. The heart of the program is the 
* recursive call that tests different possible to solutions to see if it 
* solves the puzzle. When a solution is found is the printed int the exact 
* same format the inputs are read. 
* Rules: The puzzle is a 9X9 standard Sudoku puzzle. The solve the possible
*        combinations of numbers from 1-9 that fill the board so that
         there's no repeating numbers on the same row, col, or miniGrid.
         A miniGrid is defined as the 3X3 matriz within the 9x9 puzzle.                                                                  
/****************************************************************************/
#include <stdio.h>

#define MAX_ROW 9
#define MAX_COL 9
#define MINI_RMAX 3
#define MINI_CMAX 3
#define YES 1
#define NO 0

char tempCharArray[MAX_ROW];//is the temporary array
char puzzle[MAX_ROW][MAX_COL];//2d array represent the puzzle
char tempSolution[MAX_ROW][MAX_COL];
void readPuzzle();//get digits of the puzzle
void print2array();//suppose to print 2d array
int checkForDuplicates();// check for all duplicates
void storePuzzle();//transfers the temp array into a 2 day array
int checkDupRows();//checks for duplicates in a row
int checkDupRow(int row);// checks for duplicates in all the rows
int checkDupCols();//checks for duplicates in a col
int checkDupCol(int col);// checks for duplicates all the colunms
int boxOffset();
int checkBox(int row_offset,int col_offset);
// int freeSpace(char number, int row, int col);
// int gridComplete();
int convertToNum(char x);
int solve(int row, int col);



int error=0;
int done =0;
int row = 0;
int col = 0;
int flag =0;
//main makes all the critical function calls that
//allows the puzzle to be read and prints the error message
//when the input is incorrent. It calls readPuzzle and storePuzzle.
//readPuzzle reads in and echos the input stream. storePuzzle tranfers
//the read in inputs into a 2d array that represents the puzzle.
int main(void){
  while(!done)
  {
  	if (error) 
    {
      printf("Error\n\n");
      error = 0;
    }
  	readPuzzle();
    printf("\n");
    storePuzzle();
    if(solve(row,col)) print2array();
    else printf("No Solution");
  }
}
//This functions returns nothing and is called by main.
//It's purpose is to read in input from the input stream by
//calling getchar(); function. It echos all the inputs. 
//for certain inputs outside of the range of numerical chars
//it will set a flag that there's an error so that main will 
//later print it out. It also spots invalid puzzles like
//a puzzle is too short or too long and sets a error flag.
//lastly, it sets the flag for when EOF is reached so that the
//program will stop from reading in more input.
// algorthim calls get char setting it to int c. while c is not
// a "\n" or EOF enters loop. c must not be out of range otherwise
//flag error. if c within range print it and call for next character
//after printed all the characters. Check if puzzle is valid and if
//an EOF is reached. 
void readPuzzle()
{
  int c = getchar();
  int charMax = 0;
  while (c != '\n' && c != EOF)
  {
    if (( c <'1' || c> '9') && c!='.' )
    { 
        printf("%c", c);
        error =1;
        c = getchar();
        flag =1;
    }
    if ((c >= '1' && c <= '9') || c=='.')//input must be 0-9 or '.'
    {
      tempCharArray[charMax] = c;//sets c into array at index charMax
      printf("%c", tempCharArray[charMax]);//prints the tempArray
      charMax++;
      c = getchar();
    }
  }
  if (charMax < 81 || charMax > 81) 
  {
    error = 1;//error if puzzle too long or too short
    flag = 1;
  }
  if (c == EOF) done = 1;
} 
//This fucntion does not return. It transforms the tempCharArray
//into the 2d array called puzzle. This is done by a nested while
//loop that the tempCharArray and sets it to puzzle at the current 
//nested loops index. The algorithm is multiple the row by the grid
//size (9) and add the current col. This alrightom allows 1d array
//to be represented by a 2d array. 
void storePuzzle ()
{ int i,j;
  for ( i = 0; i <MAX_ROW; i++)
  { 
    for ( j = 0; j <MAX_COL; j++)
    { 
      puzzle[i][j] = tempCharArray[9*i+j];//tranforms tempCharArray into puzzle
    }
  }
  // print2array();
}

void print2array()
{ int i,j;
  for(i = 0; i <MAX_ROW; i++)
  { 
    for( j=0; j <MAX_COL; j++) 
    {
      printf("%c", puzzle[i][j]);
      if (j == MAX_COL-1) printf("\n");
    }
  }
  checkForDuplicates();
}

int checkForDuplicates()
{ int status = NO;
  if (checkDupRows()) status = YES;
  if (checkDupCols()) status = YES;
  if (boxOffset()) status = YES;
  return status; 
}

int checkDupRows()
{ int row;
  int status = NO;

  for(row = 0; row <MAX_ROW; row++)
  {
    if (checkDupRow(row)) 
    {
      status = YES;
      flag = 1;
    }
  }
  return status;
}

int checkDupRow(int row)
{ int i, j;
  for (i = 0; i<MAX_COL; i++) 
  {
    for (j = i+1; j<MAX_COL; j++)  
    {
      if ((puzzle[row][i] == puzzle[row][j]) && puzzle[row][i]!='.')
      {
        // printf("Error row dup @ [row %d,col %d]\n", row, j);
        error= 1;
        return YES;
      }
    }
  }
  return NO;
}

int checkDupCols ()
{ int col;
  int status = NO;
  for (col = 0; col <MAX_COL; col++)
  {
    if(checkDupCol(col)) 
    {
      status = YES;
      flag = 1;
    }
  }
  return status;
}

int checkDupCol (int col)
{ int i, j;
  for (i = 0; i < MAX_ROW; i++)
  {
    for(j =1+i; j < MAX_ROW; j++)
    {
      if ((puzzle[i][col]==puzzle[j][col]) && puzzle[i][col]!='.')
      {
        // printf("Error col dup @ [row %d, col %d]\n",j, col);
        error= 1;
        return YES;
      }
    }
  }
  return NO;
}
int boxOffset()
{ int col_offset, row_offset;
  for(col_offset = 0 ; col_offset<9; col_offset +=3)
  {
    for(row_offset =0; row_offset<9; row_offset+=3)
    {
      if (checkBox(row_offset, col_offset))
      { 
        return YES;
        flag = 1;
      } 
    }
  }
 return NO;
}

int convertToNum(char x)
{ int num=0;
  if(x !='.' ||(x >= '1' && x <= '9')) num = x-'0';
  return num;
}
int checkBox( int row_offset,int col_offset)
{ int i, j, x;
  int box[10];
  for(x = 0; x <=10; x++)
  {
    box[x] = 0;
  }
  for(i =row_offset; i<row_offset+3; i++)
  {
    for(j = col_offset; j<col_offset+3; j++)
    { 
      x = convertToNum(puzzle[i][j]);
      if (box[x]== 0 &&puzzle[i][j]!='.'|| x==0)
      {
        box[x]=x;
      }
      else if(box[x]!=0 || x!=0)
      {
        error = 1;
        // printf("Error there's a duplicate%d\n", box[x]);
        return YES;
      }
    }
  }
  return NO;
}

 
// int freeSpace( char number, int row, int col)
// { char freeSpace[MAX_ROW][MAX_COL];
//   int i;
//   for (i = 0; i < MAX_ROW; i++) 
//   {
//     if (tempSolution[row][i] == number) return NO;
//     if (tempSolution[row][i] =='.') return YES;
//   }
//   for (i = 0; i < MAX_ROW; i++)
//   {
//     if (tempSolution[i][col] == number) return NO;
//     if (tempSolution[i][col] =='.') return YES;
//   } 
//   return YES;
// }
int solve( int row, int col)
{ int solution = 0;
  int i, j;
  int num;
  if(row==MAX_ROW-1)
  {
    solution =1;
    return YES;
  }
  if(col==MAX_COL-1)
  {
    row++;
  }
  if(puzzle[row][col]=='.')
  {
    for(i = 0; i <MAX_ROW; i++)
    {
      for(j = 0; j <MAX_COL; j++)
      {
        for(num= 1; num <=MAX_ROW; num++)
        {
          if(!flag && !checkForDuplicates()) puzzle[i][j] = num;
        }
      }
    }
  }
  if(flag == 0)
  {
    solution = solve(row,col+1);
  }
  if(solution == 0)
  {
    puzzle[row][col] ='.';
  }
  else
  {
    return solve(col+1, row);
  }
  return NO;
}
// void solve(int row, int col)
// {
//   char number;
//   for(row = 0; row < MAX_ROW; row++)
//   {
//     for(col = 0; col <MAX_COL; col++)
//     {
//       for (number = 1; number <= MAX_ROW; number++)
//       {
//         if (freeSpace(number,row,col))
//         {
//           tempSolution[row][col] = puzzle[row][col];
//           tempSolution[row][col] = number;
//           printf("%c", tempSolution[row][col]);
//         }
//         if(gridComplete()) return;
//       }
//     }
//   }
// }
// int gridComplete()
// {
//   int row, col;
//   for (row = MAX_ROW-1; row >=0; row--)
//   {
//     for (col = MAX_COL-1; col>=0; col--)
//     {
//       if(puzzle[row][col]=='.'|| (puzzle[row][col] >= '1' && puzzle[row][col] <= '9'))
//       {
//         return NO;
//       }
//     }
//   }
//   return YES;
// }

