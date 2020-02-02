/*************************************************************************
	> File Name: 6.11.c
	> Author: 
	> Mail: 
	> Created Time: 2020年01月31日 星期五 23时03分14秒
 ************************************************************************/

#include<stdio.h>
int main(void)
{
    int cnt;
    double sum1=0,sum2=0;
    printf("规定循环的次数：");
    scanf("%d",&cnt);
    for(int i=1;i<cnt+1;i++)
    {
        sum1=sum1+1.0/i;
        if(i%2==0)
        {
            sum2=sum2-1.0/i;
        }
        else sum2=sum2+1.0/i;
    }
    printf("1+1/2+1/3+1/4+.......=%lf\n1-1/2+1/3-1/4+.......=%lf\n",sum1,sum2);
}
