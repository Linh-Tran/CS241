/****************************************************************************
 * Linh Tran                                                                
 * Date: September 10, 2014                                                 
 * CS-241 Section # 1                                                       
 * This program read in command line arguements in main. The program
 * does binary to decimal number conversion and vice versa. It dectects
 * if the arguements entered are correct if not produce an error outcome.
 * This lab incorporates the use of pointers and conversion between 
 * number bases. Credit to Carl Norum 
 * (http://stackoverflow.com/questions/1449805/how-to-format-a-number-from-
 * 1123456789-to-1-123-456-789-in-c) 
 * for the comma separator code and 
 * binary conversion code adopted from CS241 Spring 2011 midterm #7.
/****************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <locale.h>

/*Declaration of Functions*/
void printErrorFormat(); /*print out the block error statements*/
int findPrintError(); /* print error if the size or option is incorrect*/
void convertToBinary(int base); /*converts from decimal to binary*/
long convertToDecimal(int base); /*converts from binary to decimal*/
long addDecimalCommasSpace(long numC); /*add commans to decimal numbers*/

/*Declaration of variables*/

char *size; /*pointer to the agrv[1]*/
char *option;/*pointer to the agrv[2]*/
char *num;/*pointer to the agrv[3]*/
int error = 0; /*flag error if command argc <4 */

/****************************************************
* Main expects command lines to be passed in argc,
* is the number of arguements that are passed in. 
* argv[] is the pointer to an arrry of poiners. Each
* pointer in the array is the address of the first
* char in a null terminated string. It does the most
* of the function calls to convert the numbers.
*****************************************************/ 
int main (int argc, char *argv[]) 
{
  if (argc <4) /* if the argc is less than 4 then print error*/
  { error = 1;
    if (error) 
    {
      printf("ERROR: incorrect number of arguments\n");
      /* call findPrintError to print the rest of the error statements*/
      printErrorFormat();
      error = 0;
    }
  }
  else
  {
    size = argv[1]; /*assign the address argv[1] to size*/
    option= argv[2];/*assign the address argv[2] to option*/
    num = argv[3]; /*assign the address argv[3] to num*/
    
    /*compare the the value that option points to is "-d" or "-b"
      if it is "-d" then will call convertTodBinary with a base of 
      10. Else of call convertToDecimal with a base of 2. Else
      will print eror format*/
    if (findPrintError())
    {
      if(strcmp(option, "-d")==0) convertToBinary(10);
      else if (strcmp(option, "-b")==0) convertToDecimal(2);
      else findPrintError();
    }
  }  
}

/****************************************************
 * Function name: convertToBinary.
 * Parameters int base passed from main.
 * Does not return any value.
 * Helper function get called by main with a passed 
 * int base value that a argv[3] points to so that
 * function could convert the string to long int 
 * using stroul. Use bit shifitng and 
 * masking to convert a long decimal into binary.
 * and store the converted value in a char[] to be
 * printed.
*****************************************************/ 
void convertToBinary(int base)
{ 
  char *ref; /*pointer to the string inside argc[3]*/ 
  long numC; /*converted char value of the argc[3]*/
  /*determine number of spaces needed inside the binary[]*/
  int numSpace; 
  int j=0;
  int k=0;
  /* flag var that decides the size of the binary number 
     it is converted to*/
  long bits_len = 0;
  long new_length; /* holds the size of the binary [] including spaces*/
  
  /* the position where the bit shifting starts.*/
  unsigned long p = 0;
   /*flag telling you how many bits (aka array length)
  / for example: 0 - 255 are 8-bit numbers. 256 - ... are 16-bit*/
  if(strcmp(size, "-8")==0) bits_len = 8; 
  if(strcmp(size, "-16")==0) bits_len =16;
  if(strcmp(size, "-32")==0) bits_len = 32;
  if(strcmp(size, "-64")==0) bits_len = 64;

  numSpace = bits_len/4;/* two spaces if bits_len is 8. 0000_0000_*/
  new_length = bits_len + numSpace;/*calculates the new length of array*/
  char binary[new_length];
  binary[new_length-1] = '\0';/*null terminate the last char in the char[]*/
 
  /* converts the string pointed to by ref to a long int*/
  numC = strtoul(num, &ref, base);
  if(*ref!='\0') /*if the numbers found in numC is not a number, than print erro*/
  {
    printf("ERROR: argument 3 is not a decimal integer\n");
    printErrorFormat();
  }
  else
  { 
    k = new_length -2; /*the current variable looking to shift*/
    /*continue to convert as long the number is < length desired*/
    for (j=0; j<bits_len; j++) 
    {
      /*shifts the position looking at over to left each loop*/
      p=1L<<j;
      /* if the position and number agreees then set the current variable
         the position it needs to be inside the binary [].*/
      if (numC & p) binary[k] = '1'; 
      else binary[k] = '0';/* else set the binary [] at p location to 0*/
      /* every 4 indices of the binary [] add a space and decrement k so
         the spaces are not counted as numbers*/
      if ((j+1) % 4 == 0) binary[--k] = ' ';
      k--; /* decrement the numbers of values need to be converted*/
    }
    /* print the converted binary []*/
    printf("%s \n", binary);
  }
}
/****************************************************
 * Function name: convertToDecimal
 * Parameters int base passed from main.
 * Return a long int value for adding commas and spaces
 * Helper function get called by main with a passed 
 * int base value that a argv[3] points to so that
 * function could convert the string to long int 
 * using stroul. Use bit shifitng and 
 * masking to convert a long binary into decimal.
 * and prints out the converted number.
*****************************************************/ 
long convertToDecimal(int base)
{ 
  char *ref; /*pointer to the string inside argc[3]*/
  long numC = 0; /*converted char value of the argc[3]*/
  /* converts the string pointed to by ref to a long int*
     automatic conversion form binary to decimal, base is 2*/
  numC = strtoul(num, &ref, base);
  /*if the numbers found in numC is not a binary, than print erro*/
  if(*ref>1) 
  {
    printf("ERROR: argument 3 is not a binary integer\n");
    printErrorFormat();
  }

  else addDecimalCommasSpace(numC);/* call the add commas to the number*/
  return numC;
}

/****************************************************
 * Function name: addDecimalCommasSpace
 * Parameters long numC passed from convertToDecimal.
 * Helper function get called by convertToDecimal and
 * use strmp to see how many spaces and commas to add
 * to the decimal number. 
 * and prints outs the formated decimal number.
 * use setlocale and width trick of printf to set
 * the commas and spaces.
*****************************************************/
long addDecimalCommasSpace (long numC)
{
  int space;
  /*sets the space according the the base*/
  if(strcmp(size, "-8")==0) space = 3; 
  if(strcmp(size, "-16")==0) space =6;
  if(strcmp(size, "-32")==0) space =13;
  if(strcmp(size, "-64")==0) space =25;

  char stringNum [space];
  
  setlocale(LC_NUMERIC, "");
  sprintf(stringNum,"%'li",numC);
  printf("%*s\n",space, stringNum);
}

/****************************************************
 * Function name: findPrintError
 * No Parameters and return int for main to see if
 * there's an error at the start.
 * Helper function get called by main to make sure
 * that the size and option are correct. If not 
 * correct prints error. Uses strcmp fucntion to
 * compare the strings that size and option points to.
*****************************************************/
int findPrintError()
{
  if (strcmp(size,"-8")!=0 && strcmp(size,"-16")!=0 
      && strcmp(size,"-32")!=0 && strcmp(size,"-64")!=0)
  {
    printf("ERROR: argument 1 must be: -8 | -16 | -32 | -64\n");
    printErrorFormat();
    return 0;
  }
  if(strcmp(option,"-d")!=0 && strcmp(option,"-b")!=0)
  {
    printf("ERROR: argument 2 must be -b | -d\n");
    printErrorFormat();
    return 0;
  }
  return 1;
}

/****************************************************
 * Function name: printErrorFormat
 * No Parameters and no return.
 * Helper function get called by main or
 * findPrintError() whenever there's
 * an error with the input like size, option, or num.
*****************************************************/
void printErrorFormat()
{
  printf("usage:\n");
  printf("binary SIZE OPTION NUMBER\n");
  printf("  SIZE:\n");
  printf("    -8   input is an unsigned 8-bit integer.\n");
  printf("    -16   input is an unsigned 16-bit integer.\n");
  printf("    -32   input is an unsigned 32-bit integer.\n");
  printf("    -64   input is an unsigned 64-bit integer.\n");
  printf("\n");
  printf("  OPTION:\n");
  printf("    -b   NUMBER is binary and output will be in decimal.\n");
  printf("    -d   NUMBER is decimal and output will be in binary.\n");
  printf("\n");
  printf("  NUMBER:\n");
  printf("    number to be converted.\n");
  printf("\n");
}








