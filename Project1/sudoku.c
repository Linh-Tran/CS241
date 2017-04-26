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

/*Defined constants*/
#define MAX_ROW 9 
#define MAX_COL 9
#define MINI_RMAX 3
#define MINI_CMAX 3 
#define YES 1 //YES when there's a duplicate 
#define NO 0 //NO when there's no duplicate

/*Declaration of fucntions*/

char puzzle[MAX_ROW][MAX_COL];/* 2d array represent the puzzle */
char tempCharArray[MAX_ROW];/* is the temporary array */
void readPuzzle();/* read in characters for the puzzle */
void print2array();/* format the print 2d array */
int checkForDuplicates();/* checks for all duplicates */
void storePuzzle();/* transfers the temp array into a 2 day array */
int checkDupRows();/* checks for duplicates in a row */
int checkDupRow(int row);/* checks for duplicates in all the rows */
int checkDupCols();/* checks for duplicates in a col */
int checkDupCol(int col);/* checks for duplicates all the colunms */
int boxOffset(); /* locate the positions of each of the 3x3 boxes on the grid */
/* checks for duplicates in each 3x3 box */
int checkBox(int row_offset,int col_offset); 
/* convert a character from the board into the integer representation */
int convertToNum(char x); 
int solve(int row, int col); /* solve the puzzle recursively */


/*Declaration of variables*/

int error=0; /*prints "Error" when equal 1 */
int done =0; /*stops program when done */

/* initializes row for the solve() */
int row = 0; 
int col = 0;

/**********************************************************************
* main makes all the critical function calls that
* allows the puzzle to be read and prints the error message
* when the input is incorrect. It calls readPuzzle and storePuzzle.
* readPuzzle reads in and echos the input stream. StorePuzzle tranfers
* the read in inputs into a 2d array that represents the puzzle. 
* It calls prints the solution or No solution when solve() is finished
* solving the puzzle.
*********************************************************************/
int main(void){
  /* main will run as long as done is not 0 */
  while(!done)
  {
    /*print error when the flag error = 0 */
  	if (error) 
    {
      printf("Error\n");
      error = 0;
    }
  	readPuzzle(); /* calls readPuzzle() */
    printf("\n");
    storePuzzle();/*calls storePuzzle() after read in puzzle */
    print2array();

    /*checks puzzle for duplicates before solving*/
    if(checkForDuplicates()) error = 1; 
    
     /* if there's no error or dupplicated
        call to solve() with intialized 0 row and col 
        if there's a solution
        print the puzzle. Else prints "No Solution" */
    if(!error && !checkForDuplicates())
    {
      if(solve(row,col)) print2array();
      else printf("No Solution\n");
    }
  }
}
/*********************************************************************
* Function name: readPuzzle
* No parameters and does not reuturn anything is called by main
* Helper function read in characters from getchar()
* input that is not a dot or number from 1-9 will print error
* It echos all the inputs. 
* It also spots invalid puzzles like a puzzle is too short or too long 
* and sets a error flag.
* Lastly, it sets the flag for when EOF is reached so that the
* program will stop from reading in more input.
* Algorthim: calls get char setting it to int c. while c is not
* a "\n" or EOF enters loop. c must not be out of range otherwise
* flag error. if c within range print it and call for next character
* after printed all the characters. Check if puzzle is valid and if
* an EOF is reached.
*********************************************************************/
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
    }
    if ((c >= '1' && c <= '9') || c=='.')/* input must be 0-9 or '.'*/
    {
      tempCharArray[charMax] = c;/*sets c into array at index charMax*/
      // printf("%c", tempCharArray[charMax]);/* prints the tempArray */
      charMax++;
      c = getchar();
    }
  }
  /*error if puzzle too long or too short*/
  if (charMax < 81 || charMax > 81) error = 1;

  if (c == EOF) done = 1; /*stops the program sets done to 1 when reach EOF*/
} 

/*****************************************************************
* Funtion name : storePuzzle
* No parameters, called by main, and does not return anything.
* It transforms the tempCharArray into the 2d array called puzzle. 
* This is done by a nested while loop that the tempCharArray and 
* sets it to puzzle at the current nested loops index. 
* The algorithm is multiple the row by the grid
* size (9) and add the current col. This alrightom allows 1d array
* to be represented by a 2d array. 
*****************************************************************/
void storePuzzle ()
{ int i,j;
  for ( i = 0; i <MAX_ROW; i++)
  { 
    for ( j = 0; j <MAX_COL; j++)
    { /* tranforms tempCharArray into puzzle*/
      puzzle[i][j] = tempCharArray[9*i+j];
    }
  }
}
/*****************************************************************
* Function name: print2array
* No parameters, called store puzzle, and does not return anything.
* It prints the 2d array after the the temp array transformed into
* a 2d array. 
* Algorithm loop through the rows and columns of the 2d array and
* print the input will print '/n' when done with each row of input.
* calls to check for duplicates after done.
******************************************************************/
void print2array()
{ int i,j;
  for(i = 0; i <MAX_ROW; i++)
  { 
    for( j=0; j <MAX_COL; j++) 
    {
      printf("%c", puzzle[i][j]);
    }
  }
  printf("\n");
}
/*****************************************************************
* Function name: checkForDuplicates
* No paramenters, but returns an int status 0 or 1.
* Helper method that hold the status of all the duplicates in the
* puzzle. It does that by calling 3 of the duplicate checker 
* functions. When any of them returns "YES" then there's a 
* in the puzzle else there's no duplicates "NO". 
* **************************************************************/
int checkForDuplicates()
{ 
  if (checkDupRows()) return YES;
  if (checkDupCols()) return YES;
  if (boxOffset()) return YES;
  return NO; 
}

/********************************************************************
* Function name: checkDupRows
* No paramenters, but returns an int status NO or YES.
* Helper method loops through the rows and return YES 
* when it finds a duplicate in a current row, analyzed by checkDupRow().
* returns "YES" Otherwise go to the next row and call
* checkDupRow(row) again. This fucntion will check all the rows
* for duplicates and then return status.
* ******************************************************************/
int checkDupRows()
{ int row;
  int status = NO;

  for(row = 0; row <MAX_ROW; row++)
  {
    if (checkDupRow(row)) 
    {
      return YES;
    }
  }
  return NO;
}
/*********************************************************************
* Function name: checkDupRow
* Paramenter is row representing which row it should look it
* Row will not change until the next function call by checkDupRows()
* that provides the next row to check. Returns an int status NO or YES
* when it finds duplicate in the current row.
* Algorithm search through a constant row selected by the checkDupRows
* will return YES the moment it comes across a duplicate otherwish
* will be called again provided a different row until done checking the
* puzzle.
* ********************************************************************/
int checkDupRow(int row)
{ int i, j;
  for (i = 0; i<MAX_COL; i++) 
  {
    for (j = i+1; j<MAX_COL; j++)  
    {
      if ((puzzle[row][i] == puzzle[row][j]) && puzzle[row][i]!='.')
      {
        // printf("Error row dup @ [row %d,col %d]\n", row, j);
        return YES;
      }
    }
  }
  return NO;
}

/********************************************************************
* Function name: checkDupCols
* No paramenters, but returns an int status NO or YES.
* Helper method loops through the cols and return YES 
* when it finds a duplicate in a current col, analyzed by checkDupCol().
* returns "YES" Otherwise go to the next col and call
* checkDupCol(col) again. This fucntion will check all the cols
* for duplicates and then return status.
* ******************************************************************/
int checkDupCols ()
{ int col;
  int status = NO;
  for (col = 0; col <MAX_COL; col++)
  {
    if(checkDupCol(col)) 
    {
      return YES;
    }
  }
  return NO;
}

/*********************************************************************
* Function name: checkDupCol
* Paramenter is colrepresenting which col it should look it
* Col will not change until the next function call by checkDupCols()
* that provides the next col to check. Returns an int status NO or YES
* when it finds duplicate in the current col.
* Algorithm search through a constant col selected by the checkDupcols
* will return YES the moment it comes across a duplicate otherwish
* will be called again provided a different col until done checking the
* puzzle.
* ********************************************************************/
int checkDupCol (int col)
{ int i, j;
  for (i = 0; i < MAX_ROW; i++)
  {
    for(j =1+i; j < MAX_ROW; j++)
    {
      if ((puzzle[i][col]==puzzle[j][col]) && puzzle[i][col]!='.')
      {
        // printf("Error col dup @ [row %d, col %d]\n",j, col);
        return YES;
      }
    }
  }
  return NO;
}
/**********************************************************************
* Function name: boxOffset
* No paramter and returns an int YES if there's a duplicate otherwise
* NO if no duplicate.
* Function splits the board into 9 indivdual boxes. There's two
* variables that keep track of the offset call col_offset and row_offset
* each box is 3 spaces appart so the nested for loop process the rows
* of boxes then shifts the cols offset. The bounds of each box is set
* by the previous col_offset or row_offset plus three. 
* After the bounds of a box is set it will call checkBox with the
* the bounds of the box row_offset and col_offset to check the indivdual
* boxes. If checkBox returns yes then boxOffset will reuturn YES otherwise 
* NO. YES represent there's a duplicate.
* *********************************************************************/
int boxOffset()
{ int col_offset, row_offset;
  for(col_offset = 0 ; col_offset<9; col_offset +=3)
  {
    for(row_offset =0; row_offset<9; row_offset+=3)
    {
      if (checkBox(row_offset, col_offset))
      { 
        return YES;
      } 
    }
  }
 return NO;
}

/**********************************************************************
* Function name: checkBox
* Parameters character x of the puzzle
* Returns an char x as a integer called num.
* Function is called in the checkbox to check if there's a duplicate
* in the 3x3 box. The value returned will act as the index of which it 
* is stored inside of box[] to check for duplicates.
* char x must not be '.' or any other character other than 1-9.
* Algorithm substract of the '0'. 
* *********************************************************************/

int convertToNum(char x)
{ int num=0;
  if(x !='.' ||(x >= '1' && x <= '9')) num = x-'0';
  return num;
}

/**********************************************************************
* Function name: checkBox
* Parameters tow ints row_offset and col_offset
* Returns an int YES if there's a duplicate otherwise
* NO if no duplicate.
* Function is called with the bounds of the box row_offset and 
* col_offset to check an indivdual box. 
* Algorithm: Given the bounds to check for duplicates
* the fucntion checks for duplicates by using single int array box [] 
* that stores the the current character (x) checked in the puzzle at
* the x index,after. If the box[] at spot x is empty = 0, continue to 
* search else will print error because the box[] at position 
* x already has the x.
* *********************************************************************/
int checkBox( int row_offset,int col_offset)
{ int i, j, x;
  
  /* 1 array size 10 so that it stores x (character of current spot of 
    puzzle) in the x index of box[]. */
  int box[10]; 
  /* for loop through the array and make all the spaces equal to 0. */
  for(x = 0; x <=10; x++)
  {
    box[x] = 0;
  }
  /* for loop through the puzzle at the given boundaries of a box
     and store the characters into the box[] if the character is 
     not '.' and the box is empty at that x index or x is 0. */
  for(i =row_offset; i<row_offset+3; i++)
  {
    for(j = col_offset; j<col_offset+3; j++)
    { 
      /* converts the character to int */
      x = convertToNum(puzzle[i][j]); 
      if (box[x]== 0 &&puzzle[i][j]!='.'|| x==0)
      {
        box[x]=x;
      }
      /* if box is full or x is not 0 then there's a duplicate*/
      else if(box[x]!=0 || x!=0)
      {
        // printf("Error there's a duplicate%d\n", box[x]);
        return YES;
      }
    }
  }
  return NO;
}

/**********************************************************************
* Function name: solve
* Parameters two ints row and col
* Returns an int YES if there's a solution to a puzzle otherwise NO.
* Function is called by main at the passed it row and col of 0.
* Algorithm:
* Check for solutions in 9 columns of a row, when col == 9 then
* check the next row (row++). 
* If all the rows are checked (row ==MAX_ROW)
* then set the solution to 1 (meaning we have a solution and return YES)
* to main. 
* If there's an empty spot in the puzzle try different values 1-9
* assign the value to the spot. Check if that value is valid in that spot
* (no duplicates). Then recursively call solver again the spot 1 col over.
* if there's a solution when done then return YES.
* if there's no solution with all the possiblities then return NO.
* if there's no empty spot then call solve with the next col over.
* *********************************************************************/
int solve( int row, int col)
{ 
  int solution = 0; /*solution is the state of the puzzle when solving*/
  int num; /*variable possible solutions of a square '1'-'9'*/
  
  /*when done with all the colunms in a row increment row */
  if(col==MAX_COL) 
  {
    row++;
    col=0; /*sets the col zero, checks left to right*/
  }

  /* when done solving all the rows return YES signifies puzzle is solved*/
  if(row==MAX_ROW)
  {
    solution =1;
    return YES;
  }
  
  /*if sees a empty spot '.' then try plugging in values '1'-'9'*/
  if(puzzle[row][col]=='.')
  {
    for(num= '1'; num <='9'; num++)
    { 
      /*sets a possible solution into the current empty spot*/ 
      puzzle[row][col] = num;
      /*checks if that possible solution works (no duplicates)
        then try solving the next spot over*/ 
      if(!checkForDuplicates()) solution = solve(row,col+1);
      
      /* when the puzzled is solved then return YES*/
      if (solution) return YES;
    }
    /*unsovlable puzzle return NO*/
    puzzle[row][col] ='.'; /*sets back puzzle to '.' if not a solution*/
    return NO;
  }
  /*if spot is not empty try solving the next spot over*/
  else return solve(row, col+1);
}