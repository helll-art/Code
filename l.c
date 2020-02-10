/*************************************************************************
	> File Name: l.c
	> Author: 
	> Mail: 
	> Created Time: 2020年02月09日 星期日 13时57分39秒
 ************************************************************************/

#include<stdio.h>
int jisuan(int n)
{
    return n*(n-1)/2;
}
int main(void)
{
    int a,b,n;
    while(scanf("%d",&n)!=EOF)
    {
        a=n/2;
        b=n-a;
        printf("%d\n",jisuan(a)+jisuan(b));
    }
}
