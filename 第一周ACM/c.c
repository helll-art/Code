/*************************************************************************
	> File Name: c.c
	> Author: 
	> Mail: 
	> Created Time: 2020年02月08日 星期六 17时33分36秒
 ************************************************************************/

#include<stdio.h>
int main(void)
{
    int n,sum;
    int a[100];
    while(scanf("%d",&n)!=EOF)
    {
        sum=0;
        for(int i=0;i<n;i++)
        {
            scanf("%d",&a[i]);
            sum=sum+a[i];
        }
        printf("%d\n",sum);
    }
}
