// #include <stdio.h>
// #define ENDCODE -1
// int binarySearch(int x, int v[])
// {
// 	int mid;
// 	int low = 0;
// 	int high=0;
// 	while ( v[high] != ENDCODE)high++;
// 	high--;
// 	printf("%d", high);
// 	while (low <=high)
// 	{
// 		mid = (low+high)/2;
// 		printf("[%d %d %d]", low, mid, high);

// 		if(x<v[mid]) high = mid-1;
// 		else if (x>v[mid]) low = mid+1;
// 		else return mid;
// 	}
// 	return -1;
// }
// void main(void)
// {
// 	int nums[] = {12,13,15,17,21,23,27,39,43,51,-1};
// 	printf("idx = %d\n", binarySearch(21,nums));
// 	printf("idx = %d\n", binarySearch(30,nums));
// }

// #include <stdio.h>
// void main(void)
// {
// 	char s[]="Julzzzizzaz";
// 	char del='z';
// 	int srcIdx = 0, snkIdx =0;
// 	while(s[srcIdx])
// 	{
// 		if(s[srcIdx]!= del)
// 		{
// 			s[snkIdx]=s[srcIdx];
// 			snkIdx++;
// 		}
// 		else
// 		{
// 			printf("[%d, %d] %s\n", srcIdx, snkIdx,s);
// 		}
// 		srcIdx++;
// 	}
// }

#include <stdio.h>
int atoiVariant(char s[])
{
	int i = 0;
	int n=0;
	int sign = 1;

	while (s[i]== ' ' || s[i] == '\t') i ++;
	if (s[i] == '-') sign = -1;

	while (s[i])
	{
		if (s[i] >= '0' && s[i] <= '9')
		{
			int d = s[i]-'0';
			n =10*n +d;
			printf("d=%d, n=%6d\n", d, n );
		}
		i++;
	}
	return sign*n;
}

void main(void)
{
	char str[] = " -98-76WW21";
	printf("[%s] = %d\n", str, atoiVariant(str)); 
}