/*************************************************************************
	> File Name: 6.9.c
	> Author: 
	> Mail: 
	> Created Time: 2020年01月31日 星期五 22时48分52秒
 ************************************************************************/

#include<stdio.h>
int SUM(int a,int b)
{
    int sum=0;
    for (int i=a;i<b+1;i++)
    {
        sum=sum+i*i;
    }
    return (sum);
}
int main(void)
{
    int a,b;
    printf("Enter lower and upper integer limits:");
    while(scanf("%d%d",&a,&b) == 2 && a<b){
        printf("The sums of the squares from %d to %d is %d\n",a,b,SUM(a,b));
        printf("Enter lower and upper integer limits:");
    }
    printf("Done\n");
}
