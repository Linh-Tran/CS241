#include <stdio.h>
static long long unsigned lcg_m;
static long long unsigned prime[15];
int setUniquePrimeFactors(long long unsigned n)
{ 
  int testDivisor=2;
  int count=0;
  int maxPrimes=15;
  int i;
  for(i=0; i<15; i++)
  {
    prime[i] = 1;
  }
  while(1)
  {
    // if((testDivisor*testDivisor>n) && (n % testDivisor == 0))
    if((testDivisor*testDivisor>n))
    {
      if(n!=1)
      {
        if(count<maxPrimes)
        {
          prime[count]=n;        
          count++;
        }
        else return count;
      }
      return count;
    }
    while((n%testDivisor)==0)
    {
      if(count<maxPrimes)
      {
        prime[count]=testDivisor;
        while((n%testDivisor)==0 ) 
        {
          n = n/testDivisor;
        }
        count++;
      }
    }
    testDivisor++;
  }
}

int main()
{
  int i;
  long long unsigned j = 1;
  setUniquePrimeFactors();
  for (i=0;i<15;i++){
    printf("%llu\n",prime[i]);
    j = j*prime[i];
  }
  printf("%llu j is:\n",j);

  return 0;
}
