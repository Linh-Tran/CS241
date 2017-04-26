/***********************************************************************/
/* Linh Tran                                                           */   
/* Date: November 3, 2014                                              */
/* CS-241 Section # 1                                                  */
/* Program: Lejo Cipher program                                        */
/*                                                                     */
/* This program reads input from the input stream and decrypt or       */
/* encrypts data base in the input. The encryption is adopted from     */
/* Joel Castellanos. Program will print an error if the input does     */
/* Not follow the required format.                                     */
/*                                                                     */
/***********************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h> /* provides EXIT_SUCCESS */

#define DEBUG 0
#define MAP_LENGTH 28

enum cipherModeEnum {ENCRYPT, DECRYPT};
enum cipherModeEnum cipherMode;

/*
The different status codes are flags.
Since each is a power of 2, they can be turned on or off independently.
*/
int status;
const int CLEAR = 0;
const int OK = 1;
const int END_OF_LINE = 2;
const int END_OF_FILE = 4;
const int ERROR = 8;

long unsigned lcg_c; 
static long unsigned lcg_m;
long unsigned lcg_a;
long unsigned lcg_x;
int cipherMap[MAP_LENGTH];
static long unsigned prime[15];

/******************************************************************************/
/* readPositiveLong(char delimiter)                                           */
/*   Reads characters from the standard input stream until either             */
/*   the character read equals the delimiter or until an error.               */
/*                                                                            */
/*   This is a reusable utility function.                                     */
/*   It does not access any global variables.                                 */
/*   It can be moved to a different file.                                     */
/*                                                                            */
/*   An error occurs if:                                                      */
/*    1) A character is read that is not a digit (0-9) and not the delimiter. */
/*    2) More than 20 digits are read                                         */
/*    3) The compiler's representation of an unsigned long is <64 bits.       */
/*                                                                            */
/*   Returns                                                                  */
/*    If no error, returns an unsigned long defined by the digits read.       */
/*    If an error, returns 0.                                                 */
/******************************************************************************/
unsigned long readPositiveLong(char delimiter)
{
  unsigned long num = 0; /* function returns this var if there's no errors*/
  unsigned long temp=0; /* helper temp var to store the char input value*/
  char c = getchar();
  /* counts the digits read, start @ 1 b/c getchar() is already 
  called once*/
  int count = 1; 

 /* issures that long is never defined over 64 bits*/
  if(sizeof(long)>8) return 0; 
  while (c>= '0' && c <= '9')
  { 
    num = (c - '0')+ temp*10; /* char to int conversion.*/
    temp=num;
    c = getchar();
    count++;
  } 
  /* once reaches delimiter will reintialize temp and return num*/
  if (c == delimiter) 
  { 
    temp = 0;
    return num;
  }
  else return 0;
  if (count>20) return 0; /*restaints the amount of input*/
}
 
/**********************************************************************/
/*  setUniquePrimeFactors                                             */
/*    This is a reusable utility function.                            */
/*    It does not access any global variables.                        */
/*    It can be moved to a different file.                            */
/*                                                                    */
/* Parameters:                                                        */
/*    n:         Number of which the unique prime factors are found.  */
/*    prime[]:   pointer to output array of unique prime numbers.     */
/*                  Each element of prime[] from 0 to maxPrimes-1 is  */
/*                  initialized to 0.                                 */
/*    maxPrimes: Size of prime[] array.                               */
/*                                                                    */
/* Returns:                                                           */
/*    The number of unique prime factors placed in prime[].           */
/*    0 if there is an error.                                         */
/**********************************************************************/
int setUniquePrimeFactors(long unsigned lcg_m)
{  
  int testDivisor=2;
  int count=0;
  int maxPrimes=15;
  int i;
  for(i=0; i<15; i++) prime[i] = 1; /* intialize prime array to 1*/
  
  /* Generates unique primes by algorithm from slide 45 of the 
     cs241lecture22-cipher. Prime numbers is called count. Will
     Find the unique primes of the lcg_m until testDivisor is not
     longer a factor of lcg_m. Primes are stored in an unsigned long
     array called prime */
  while(1)
  {
    if((testDivisor*testDivisor>lcg_m)) 
    {
      if(lcg_m!=1)
      {
        if(count<maxPrimes)
        {
          prime[count]=lcg_m;        
          count++;
        }
        else return count; /* condition which breaks out of infinite while*/
      }
      return count;
    }
    /*Continues to divide lcg_m by the testDivisor until no longer a factor*/
    while((lcg_m%testDivisor)==0)
    {
      if(count<maxPrimes)
      {
        prime[count]=testDivisor;
        while((lcg_m%testDivisor)==0 ) 
        {
          lcg_m = lcg_m/testDivisor;
        }
        count++;
      }
    }
    testDivisor++; /* if testDivisor no longer a factor increment it*/
  }
}

/********************************************************************************/
/*  calculateLCG_a(unsigned long LCG_m)                                         */
/*    Given m of a Linear Congruential Generator (LCG): Xn+1 = (aXn + c) mod m  */
/*      This method finds an a that will yield a full period.                   */
/*      The value of a is:                                                      */
/*        a = 1+2p, if 4 is a factor of m, otherwise, a = 1+p.                  */
/*        p = (product of m's unique prime factors).                            */
/*        0 < a < m (otherwise error).                                          */
/*                                                                              */
/*    This is a reusable utility function.                                      */
/*    It does not access any global variables.                                  */
/*    It can be moved to a different file.                                      */
/*                                                                              */
/* Parameters:                                                                  */
/*    LCG_m:     m of an LCG.                                                   */
/*                                                                              */
/* Returns:                                                                    */
/*    a of the LCG or 0 if there was an error.                                  */
/********************************************************************************/
unsigned long calculateLCG_a(unsigned long LCG_m)
{
  int i;
  long unsigned p = 1;
  long unsigned LCG_a = 0;
  setUniquePrimeFactors(LCG_m);
  for(i = 0; i<15; i++) p*=prime[i]; /* find the product or the unique primes*/
  if (LCG_m%4 == 0) LCG_a = 1 + (2*p);
  else LCG_a = 1 + p;
  if (0 > LCG_a || LCG_a > LCG_m) return 0;
  return LCG_a; /*returns an unsigned long var LCG_a*/
}


/**********************************************************************/
/* skipToEndOfLine(void)                                              */
/* Helper function that reads characters from the                     */
/* standard input stream (stdin) until '\n' or EOF.                   */
/*                                                                    */
/* None of the characters read are saved.                             */
/*                                                                    */
/* If '\n' is read, sets the END_OF_LINE flag of global field status. */
/* If EOF is read, sets the END_OF_FILE flag of global field status.  */
/**********************************************************************/
void skipToEndOfLine(void)
{ 
  char c = getchar();
  /* find from stdin until find \n or EOF then sets status accordingly*/
  while(c!='\n' && c!=EOF) c = getchar(); 
  if(c == '\n') status = END_OF_LINE;
  else if(c == EOF) status = END_OF_FILE;
}


/******************************************************************************/
/*  readDataBlock(char data[])                                                */
/*    Reads one block of cipher data from the standard input stream.          */
/*    Reading stops when a full block is read or when '\n' is read.           */
/*    An error is triggered if any byte code (other than '\n') is read        */
/*      that is not a printable ASCII character: [32, 126].                   */
/*                                                                            */
/* Parameters:                                                                */
/*    data[]: a null-terminated array of size 5 into which the data is read.  */
/*      All elements of data are initialized to '\0'.                         */
/*      If global variable cipherMode == ENCRYPT, then each legal character   */
/*        read is copied into data[].                                         */
/*      If global variable cipherMode == DECRYPT, then character codes [0,31] */
/*        and 127 might be represented as two-byte codes starting with '+'.   */
/*      This function converts any such two-character codes to the single     */
/*        ASCII code [0,127]. Therefore, this function may read as many as    */
/*        eight characters form the standard input stream.                    */
/*                                                                            */
/* Returns:                                                                   */
/*   OK | END_OF_LINE | END_OF_FILE | ERROR                                   */
/******************************************************************************/
int readDataBlock(char data[])
{ 
  int i;
  for(i = 0; i<4; i++) data[i] = '\0'; /* presets data to null terms*/
  char c;

  /* Decides whether to ENCRYPT or DECRYPT base on the 1st char read*/
  /* When ENCYPT just store the vars inside data array as long as
     input is whether printable range, not '\n' or EOF */
  if(cipherMode ==ENCRYPT)
  {
    for(i =0; i<4; i++)
    {   
      c = getchar();
      if(c==EOF) return END_OF_FILE;
      if(c=='\n') return END_OF_LINE;
      if (c<' '|| c>'~') return ERROR;
      data[i] = c; 
    }
  }
  /* DECRYPT read more than the standard 4 chars to compensate
     for the excessive chars read we pick and choose which vars are
     stored. When a '*' appears we adjust the values stored.

     Process reads the chars until reaches a '*' then read the
     next char and store that values according to the rules on
     lecture slide 34. */
  if(cipherMode == DECRYPT)
  {
    for(i = 0; i<4; i++)
    { 
      c = getchar();
      if(c==EOF) return END_OF_FILE;
      if(c=='\n') return END_OF_LINE;
      if (c<' '|| c>'~') return ERROR;
      else if(c=='*') 
      { 
        c = getchar();
        if(c==EOF) return END_OF_FILE;
        if(c=='\n') return END_OF_LINE;
        if (c<' '|| c>'~') return ERROR;
        if(c=='%') c= 127;
        else if(c!='*') c= c-'?';
      }
      data[i] = c;
    }    
  }
    return OK;
}


/***********************************************************************/
/* readCipherMode(void)                                                */
/*   Reads one character from the standard input stream.               */
/*   Sets the global variable cipherMode to ENCRYPT or DECRYPT if the  */
/*     character read is an 'e' or a 'd'.                              */
/*                                                                     */
/* Returns:                                                            */
/*   OK if an 'e' or 'd' was read.                                     */
/*   END_OF_LINE if '\n' was read.                                     */
/*   END_OF_FILE if EOF was read.                                      */
/*   otherwise ERROR.                                                  */
/***********************************************************************/
int readCipherMode(void)
{ 
  /* determines whether a cipher should encyrpt or decyrpt.*/
  int c = getchar();
  if (c =='e') cipherMode = ENCRYPT;
  else if (c=='d') cipherMode = DECRYPT;
  if(c =='e' || c=='d') return OK;
  else if (c=='\n') return END_OF_LINE;
  else if (c== EOF) return END_OF_FILE;
  
  return ERROR;
}

/****************************************************************/
/* readKey(void)                                                */
/*                                                              */
/*   Calls readPositiveLong and sets the global variable lcg_m. */
/*   Calls readPositiveLong and sets the global variable lcg_c. */
/*   Calls calculateLCG_a and sets the global variable lcg_a.   */
/*   Sets the global variable lcg_x.                            */
/*                                                              */
/* Return: OK | ERROR                                           */
/****************************************************************/
int readKey(void)
{ 
  lcg_m = readPositiveLong(','); /* calls the readPositiveLong function*/
  lcg_c = readPositiveLong(',');
  lcg_a = calculateLCG_a(lcg_m);

  lcg_x = lcg_c;
  /* conditions of which the status is OK base on slide 17*/
  if(lcg_m > 0 && lcg_a < lcg_m && lcg_c <lcg_m 
     && lcg_a > 0 && lcg_c > 0) return OK;
  return ERROR;
}


/***************************************************************************/
/* buildMap(void)                                                          */
/*                                                                         */
/* Uses the global variables lcg_a, lcg_c, lcg_m and lcg_x to define the   */
/*   global array: cipherMap[] such that cipherMap[i] = k indicates that   */
/*   on encryption, bit i is moved to bit k and the reverse on decryption. */
/*                                                                         */
/* When this function returns, lcg_x will have been updated 28 steps       */
/*   in the LCG.                                                           */
/*                                                                         */
/* This method does not return a value because there is no reason for it   */
/*   to fail.                                                              */
/***************************************************************************/
void buildMap(void)
{
  int g[MAP_LENGTH], f[MAP_LENGTH], freeSpace[MAP_LENGTH];
  int i,j,k;
  int count = -1; /* starts at 0 so that 1st counted free space is 0*/
  
  /* generates the g(i) map base on algorithm provided on slide 39 
     and the x(i)*/
  for (i=0; i<MAP_LENGTH; i++) 
  {
    g[i] = lcg_x%(MAP_LENGTH-i);
    lcg_x = ((lcg_a*lcg_x)+lcg_c)%lcg_m;
  }
  /*presets the freeSpace array to 1 indicates that it's free*/
  for(i=0; i<MAP_LENGTH; i++) freeSpace[i]=1;

  /* Find the f(i) using g(i) free spots and freeSpace*/
  for(i = 0; i <MAP_LENGTH; i++)
  { 
    count =-1;
    /*looping over free spots and counting 
      free spots until reach g(i)th free spot.
      set f[i] = j. Reset the freeSpace to 0 */
    for(j = 0; j < MAP_LENGTH; j++)
    {
      if(freeSpace[j]==1) count++;
      if(g[i]==count) 
      { 
        f[i] = j;
        freeSpace[j] =0;
        break;
      }
    }    
  }
  /* Loop through f(i) and set that into the cipherMap*/
  for(k = 0; k < MAP_LENGTH; k++) cipherMap[k] = f[k];
}
/*****************************************************************************/
/* encrypt(char data[])                                                      */
/*   Uses the global variable cipherMap to encrypt the data block in data[]. */
/*   The encrypted data is sent to the standard output stream.               */
/*   The encrypted data will always be 4 to 8 bytes long.                    */
/*   Encrypted byte codes [0,31], 127 and '+' are converted to 2-byte        */
/*     printable ASCII characters.                                           */
/*                                                                           */
/* Parameters: data[]: Must be a null terminated char[] of size 5.           */
/*                                                                           */
/* Return: OK | ERROR                                                        */
/*****************************************************************************/

int encrypt(char data[])
{ 
  int i,k;
  char cipherText[5];
  int byte, bit, count;
  /* presets the cipherText to all '\0'*/
  for(i = 0; i<5; i++) cipherText[i] = '\0';
  /* map the plainText into the cipherText using the 
     bit shifting of the bit and byte i of plainText 
     to bit and byte of f(i)*/ 
  for(i = 0; i<MAP_LENGTH; i++)
  {
    k = cipherMap[i]; /* k values inside the cipherMap aka f(i)*/
    byte = i/7; 
    bit = i%7;
    /*check data(plainText) is on at byte and bit 
      if not on set it on otherwise leave it as it*/
    if(data[byte] & (1<<bit)) cipherText[k/7] |= (1<<k%7); 
  }
  /* counts the null terms to dictate when 
     to continue printing decyrpted cipher */
  for (j = 0; j < 4; j++) if(cipherText[j] == '\0') count++;

  /*prints the encyrpted ciper according to slide 35*/
  for(i = 0; i<4 && count< 4; i++)
  {
    if(cipherText[i]>=0 &&cipherText[i] <32 ) printf("*%c",('?'+cipherText[i]));
    else if (cipherText[i]==127) printf("*%%"); 
    else if (cipherText[i]=='*') printf("**");
    else printf("%c", cipherText[i]);
  }
    
  return OK;
}

/********************************************************************************/
/* decrypt(char data[])                                                         */
/*   Uses the global variable cipherMap to decrypt the data block in data[].    */
/*   The decrypted data is sent to the standard output stream.                  */
/*   The decrypted data will always be 1 to 4 bytes long.                       */
/*   If a decrypted character is '\0' it means that the data block was a        */
/*     parcial block from the end of the line. '\0' characters are not printed. */
/*   Any other decrypted byte that is not a printable ASCII character is an     */
/*     error.                                                                   */
/*                                                                              */
/* Parameters: data[]: Must be a null terminated char[] of size 5.              */
/*                                                                              */
/* Return: OK | ERROR                                                           */
/********************************************************************************/
int decrypt(char data[])
{ 
  int i,k;
  char plainText[4];
  int byte, bit;
  /*presets the plainText to all '\0' */
  for(i = 0; i<4; i++) plainText[i] = '\0';

   /* map the cipherText into the plainText using the 
     bit shifting of the bit and byte i of cipherText 
     to bit and byte of f(i)*/ 
  for(i = 0; i<MAP_LENGTH; i++)
  { 
    k = cipherMap[i];
    byte = k/7;
    bit = k%7;
    /*check data(plainText) is on at byte and bit 
      if not on set it on otherwise leave it as it*/
    if(data[byte] & (1<<bit)) plainText[i/7] |= (1<<i%7);
  }

  /*prints the encyrpted ciper according to slide 35*/
  for(i = 0; i<4; i++)
  { 
    char c = plainText[i];
    if (c>0 && c<32|| c> 126) return ERROR; 
    if(c!='\0') printf("%c",c); 
  }  
  return OK;
}

/*****************************************************************/
/* main encrypts and decrypts lines until end of file is reached */
/*****************************************************************/
int main()
{ 
  int inputLineNumber = 0;
  status = CLEAR;

  char data[5];
  data[4] = '\0';


  while (status != END_OF_FILE)
  {
    status = CLEAR;
    inputLineNumber++;

    status = readCipherMode();
    if (DEBUG) printf("readCipherMode::mode=%d status=%d\n",cipherMode,status);

    if ((status & END_OF_FILE) == 0) 
    { 
      printf("%5d) ", inputLineNumber);
    }
    if (status == OK)
    { 
      status = readKey();
      if (DEBUG)
      { 
        printf ("\tKey: m=%lu c=%lu a=%lu x=%lu status=%d\n", 
          lcg_m, lcg_c, lcg_a, lcg_x, status);
      }
    }

    while (status == OK) 
    {
      buildMap();
      status = readDataBlock(data);
      if (DEBUG) printf("\treadDataBlock::data=%s status=%d\n",data,status);
      if ((status & ERROR) == 0)
      { 
        if (cipherMode == ENCRYPT) 
        { 
          status |= encrypt(data);
        }
        else 
        {
          status |= decrypt(data);
        }
      }
    }
    
    if (status & ERROR)
    { 
      puts("Error");
      skipToEndOfLine();
    }
    else puts("");
  }
  return EXIT_SUCCESS;
}
