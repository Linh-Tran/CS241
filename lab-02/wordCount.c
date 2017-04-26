/***************************************************************************
* Linh Tran                                                                
* Date: September 3, 2014                                                  
* CS-241 Section # 1
*
* This program reads a text file from the input stream
* and outputs the number of character, words, and lines of the read
* file called "testWords.in". The format which program prints follows 
* the text file called "testWords.Out". Each line of input is echoed 
* with a number beside it and then the input, and last the 
* "[words,Characters]". After the lines of input are echoed three extra
* lines are printed: the total of line, words, and characters of the file.
* This program fails print the correct two last lines that states
* the line which has the most amount of characters and words.  
*
****************************************************************************/
#include <stdio.h>
#define IN  1 /* inside a word  */
#define OUT 0 /* outside a word */
#define IS_MAX_LINE 1
#define NOT_MAX_LINE 0
#define MAX_CHARS 100
char currentLine[MAX_CHARS]; /* char array that keeps track of current line */
int c, numLines, numChars, numWords, state;
int totalWords, totalChars, maxChar, maxWord, maxLineState, maxLine;

main()
{ 
  /*State is out of the word*/
  state = OUT;
  /*Sets all values to zero*/
  numLines = numWords = numChars = maxChar = maxWord = 0;
while ((c = getchar())!=EOF) 
{ 
  ++numChars;
  ++maxChar;
  currentLine[numChars-1] = c;/*puts characters into array*/
  maxLineState = NOT_MAX_LINE;
  
  /*if the character is a new then will remove the /n from the
    array and decrement it from being counted. Then prints
    out the current line in correct format*/
  if (c == '\n') 
  { 
    /*put in an empty value in replace of /n */
    currentLine[numChars-1]='\0';
    numChars--;//decrement /n from the count of numChars
    maxChar--;
    ++numLines;//increment number of lines
    totalChars--;
  } 
    printf("%d)%s[%d,%d]\n",numLines,currentLine,numWords,numChars);
    //set the number of words and characters to 0 in current 
    numWords = numChars = 0;
 
  /*switch the state to outside a word when the character 
    is a space,tab, or a new line*/ 
  if (c == ' ' || c == '\n' || c == '\t')
  { 
    state = OUT; 
  }
  /*If is out of a word then, switch inside a word, increment
    the number of current words, max words, and attempt
    to store the max word for the current line*/
  else if (state == OUT)
  { 
    state = IN;
    ++numWords;
    ++maxWord;
    if(numWords > maxWord)
    {
      maxWord = numWords;
      maxLineState = IS_MAX_LINE;
    }
    if (IS_MAX_LINE)
    {
      maxLine = numLines;
      maxLineState = NOT_MAX_LINE;  
    } 
    ++totalWords;
  }
  ++totalChars;
}
  /*prints the line with most word, most characters and total 
   all the lines, characters, and lines*/
  printf("Line %d has the most words (%d)\n", maxLine, maxWord);
  printf("Line %d has the most characters (%d)\n", maxLine, maxChar);
  printf("%d lines, %d words, %d characters\n", numLines,totalWords,totalChars);
}

