/****************************************************************************
 * Linh Tran                                                                
 * Date: September 10, 2014                                                 
 * CS-241 Section # 1                                                       
 * This program read in command line arguements in main. The program
 * does binary to decimal number conversion and vice versa. It dectects
 * if the arguements entered are correct if not produce an error outcome.                                                                  
/****************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

void printErrorFormat();
void findPrintError();
void convertToBinary(int base);
long convertToDecimal(int base);
long addDecimalCommas(unsigned long num);
long addDecimalSpaces(unsigned long num);
char *size;
char *option;
char *num;
int error = 0;

int main (int argc, char *argv[]) 
{
  if (argc <4)
  { error = 1;
    if (error) 
    {
      printf("ERROR: incorrect number of arguments\n");
      findPrintError();
      error = 0;
    }
  }
  else
  {
    size = argv[1];
    option= argv[2];
    num = argv[3];
    if(strcmp(option, "-d")==0) convertToBinary(10);
    else if (strcmp(option, "-b")==0) convertToDecimal(2);
    else findPrintError();
  }  
}

void convertToBinary(int base)
{ 
  char *ref;
  long numC, binNum,rem;
  int numSpace;
  int j=0;
  int k=0;
  int bits_len = 0;
  int new_length;
  unsigned int p = 0;
   // flag telling you how many bits (aka array length)
  // for binary numbers, argv[1] is size of array
  // for decimal numbers, argv[1] describes a range of numbers
  // for example: 0 - 255 are 8-bit numbers. 256 - ... are 16-bit
  if(strcmp(size, "-8")==0) bits_len = 8; 
  if(strcmp(size, "-16")==0) bits_len =16;
  if(strcmp(size, "-32")==0) bits_len = 32;
  
  numSpace = bits_len/4;// two spaces if bits_len is 8. 0000_0000_
  new_length = bits_len + numSpace;
  char binary[new_length];
  binary[new_length-1] = '\0';
 
  numC = strtoul(num, &ref, base);
  if(*ref!='\0')
  {
    printf("ERROR: argument 3 is not a decimal integer\n");
    printErrorFormat();
  }
  else
  {
    k = new_length -2;
    for (j=0; j<bits_len; j++)
    {
      unsigned int p=1<<j;
      if (numC & p) binary[k] = '1';
      else binary[k] = '0';
      if ((j+1) % 4 == 0) binary[--k] = ' ';
      k--;
    }
    //int binarylen = strlen(binary);
    printf("%s\n", binary);
    printf("\n");
  }
 
}

long convertToDecimal(int base)
{ 
  char *ref;
  long numC = 0;
  numC = strtoul(num, &ref, base);
  if(*ref>1) 
  {
    printf("ERROR: argument 3 is not a binary integer\n");
    printErrorFormat();
  }
  else printf("%ld\n", numC);
  addDecimalCommas(numC);
  return numC;
}

long addDecimalCommas (unsigned long num)
{ 
  printf("hello");
  int dec_len = 0;
  if(strcmp(size, "-8")==0) dec_len = 8; 
  if(strcmp(size, "-16")==0) dec_len =16;
  if(strcmp(size, "-32")==0) dec_len = 32;
  if(num>1000) 
  {
    char decimal [dec_len];
    decimal [dec_len-1] = '\0';
    sprintf (decimal, "%lu" , num);
    printf("%s", decimal);
  }
}




// while(rem/2>0)
  // {
  //   rem = numC/2;
  //   binNum = numC%2;
  //   binary[i] = binNum;
  //   printf("%ld",binary[i]);
  //   i++;
  //   numC = numC/2;
  // }
  // for (j = bits-1; j>0; j--)
  // {
  //   // reverse binary array
  //   printf("%ld", binary[j]);

  // }

  // for(j = 0; j < ; j++)
  // printf("%ld\n", binNum);
  
// int formatBinary(int num)
// {
  // char bits[40];
  // bits[39] = '\0';
  // unsigned int n=400;
  // unsigned int p;
  // int i;
  // int k=38;
  // for (i=0; i<32; i++)
  // {
  // unsigned int p=1<<i;
  // if (n & p) bits[k] = '1';
  // else bits[k] = '0';
  // if ((i+1) % 4 == 0) bits[--k] = ' ';
  // k--;
  // }
  // printf("%s\n", bits);
  // }
// }


void findPrintError()
{
  if (strcmp(size,"-8")!=0 && strcmp(size,"-16")!=0 
      && strcmp(size,"-32")!=0 && strcmp(size,"-64")!=0)
  {
    printf("ERROR: argument 1 must be: -8 | -16 | -32 | -64\n");
  }
  if(strcmp(option,"-d")!=0 && strcmp(option,"-b")!=0)
  {
    printf("ERROR: argument 2 must be -b | -d\n");
  }
}

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








