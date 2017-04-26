#include <stdio.h>
unsigned long calculateLCG_a(unsigned long LCG_m);
int uniquePrimeFactor(long long unsigned n, int prime[], int maxPrimes);
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
/* Returns:                                                                     */
/*    a of the LCG or 0 if there was an error.                                  */
/********************************************************************************/
int uniquePrimeFactor(long long unsigned  n, int prime[], int maxPrimes)
{
  int testDiv =2;
  int count = 0;
  int testDivisor=2;
  int count=0;
  maxPrimes= 15;
  int prime[15];
  //n=lcg_m;
  maxPrimes=15;
  while(1)
  {
  if(testDivisor*testDivisor>n)
  {
    //printf("%d\n",);
    if(n!=1)
    {
      if(count<maxPrimes){
        prime[count]=testDivisor;
        count++;
        //printf("%d\n", testDivisor);
      }

      else return count;
    }
    return count;
  }
  while(n % testDivisor==0)
  {
    if(count<maxPrimes){
      prime[count]=testDivisor;
      count++;
    //printf("%d\n", testDivisor);
  }
    else return count;
    n /= testDivisor;
  }
  testDivisor++;
  }
}

unsigned long calculateLCG_a(unsigned long LCG_m)
{
  // int prime[15]; //global maybe?
  //slide 29 
  int error = 0;
  
  unsigned long p = uniquePrimeFactor(LCG_m, prime, 15);
  unsigned long LCG_a = 0;

  if (LCG_m%4 == 0) LCG_a = 1 + (2*p);
  else LCG_a = 1 + p;
  if (LGC_a <= 0 || LCG_a >= LCG_m) return 0;
  return LCG_a;
}

void main ()
{
  int prime[15];
  int i;
  //int maxPrimes=15;
  setUniquePrimeFactors(11682905869181336790, prime, 15);
  for (i=0;i<15;i++){
    printf("%d\n",prime[i] );  }
}