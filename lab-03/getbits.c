/****************************************************************************
 * Linh Tran                                                                
 * Date: September 10, 2014                                                 
 * CS-241 Section # 1                                                       
 * This program reads in input from the standard input stream,              
 * passes three unsigned integers into a getbits and display the result.
 * A valid record is in the follwoing format x;p;n. Each unsigned must be
 * in no larger than int of 4 bytes. The program prints "Error" base
 * on certain error cases that was tested coincide with testgetbits.out file.                                                                  
/****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
/*Declaration of functions */
unsigned long int getValue(unsigned int input); 
unsigned int getbits(unsigned int x, unsigned int p, unsigned int n);
unsigned int input; /* characters read from standard input. */
unsigned long int x; /* manipulated integer in getbits function. */ 
unsigned long int maxX = 0xffffffff; /* representation of 2^32 bytes */
unsigned int p,n; /* p is position that get bits looks at to shift n bits. */
  

int main (void)
{
   /****************************************************
   * While is infinite loop that will be false when
   * getchar reaches EOF in the helper function who
   * calls the getchar() after each character is read
   * from standard input. This loop calls getValue once
   * for each unsigned value x, p, n. Then the rest of
   * the characters are received inside getValue while.
   * When the getValue while is false, main calls next
   * sets p to getValue again with getchar 
   * called once in main.The same process works for n as 
   * well for each line of input x;p;n; until end of file
  *****************************************************/ 
	while (1)
	{
    /* sets x,p,n to the return value of getValue with
       the first character from getChar() */
		x = getValue(getchar()); 
		p = getValue(getchar());
		n = getValue(getchar());
    
    /* print "Error" when x is larger than 2^32*/
		if ( x > maxX )
    {
      printf("Error\n");
    }

    /* prints "Error" when all the values are 0 */
    else if (x==0 && p==0 && n==0)
    {
      printf("Error\n");
    }

    /* print "Error" when n is larger than p+1 
       p or n is looking at spaces outside of 31 spaces
       availible */
    else if (n > p+1 || (p> 31 || n>31) )
		{
			printf("Error\n");
		}	

    /* calls get bits passing through values x,p,n 
       and prints the final outcome as the required output format */
		else
		{
			printf("getbits(x=%lu, p=%u, n=%u) = %d\n",x,p,n,getbits(x,p,n));
		}
	} 
}
/****************************************************
 * Function name: getValue.
 * Parameters unsigned int input, ascii value of 
 * characters from getchar()
 * Returns an unsigned int num representing the
 * converted characters from getchar()
 * Helper function get called by main with a passed 
 * ascii value of character number named input. The 
 * helper makes sure the input is a number, then
 * converts the characters individully to the actual
 * unsigned int value. Calls getChar to get the next
 * number which could be x,p, or n.
 * If the input is at the end of file fuction will
 * terminate the infinite while in main.
 * num = input - '0' + num * 10;
 * Ex. number is 12. Read in the '1' convert to 1 then
 * save it to num. The first time num equal to 0 so
 * (+ num *10) is 0 won't affect final number. Then
 * read in the '2' this time the previous num is still
 * 1 so num = 12. 
*****************************************************/ 

unsigned long int getValue( unsigned int input)
{
	unsigned long int num = 0;   
  /* loops through input and converts input to 
     unsigned integer value by subtracting '0' 
     from number then num by x10 the num. Exits 
     when any input that is not a number for example
     ';' or '/n' goes back to be call by main*/
	while (input>='0' && input <='9')
	{
		num = input - '0'+ num * 10;
    /* calls getchar() and set the read characters to input. */
		input = getchar();
	}

  /* exits standard input stream if reaches end of file */
	if (input == EOF) 
    {
      exit(0);
    } 
  
  /*returns input as unsigned int called num */
	return num;
}

/************************************************************
 * Function name: getbits.
 * Parameters unsigned int x, unsigned int p, unsigned int n:
 * input x, p , n
 * Returns an unsigned reslulted from bit shifting.
 * Fucntion taken from section 2.9 of the textbook. 
 * x is the unsigned number manipulated getbits,
 * p is the position function looks at to shift
 * by n bits.
 * (x >> (p+1-n)) moves the unsigned int x to the
 * right starting at position p+1 by n bits. 
 * ~(0 << n) set the switch all the 1's to complement
 * to the left of p and mask the right remaining n
 * bits with 1s.
************************************************************/   
unsigned int getbits(unsigned int x, unsigned int p, unsigned int n)   
{       
	return (x >> (p+1-n)) & ~(~0 << n);   
}
