/*************************************************************************
	> File Name: 10.5.c
	> Author: 
	> Mail: 
	> Created Time: 2020年02月07日 星期五 19时53分48秒
 ************************************************************************/

#include<stdio.h>
void chazhi(double a[],int n)
{
    int min,max,t;
    max=a[0];
    min=a[0];
    for(int i=1;i<n;i++)
    {
        if(a[i]>=max)
            max=a[i];
        if(a[i]<=min)
            min=a[i];
    }
    t=max-min;
    printf("%d",t);
}
int main(void)
{
    double a[100];
    int n,t;
    printf("输入要输入的数据个数：");
    scanf("%d",&n);
    for(int i=0;i<n;i++)
    {
        printf("输入第%d个数据：",i+1);
        scanf("%lf",&a[i]);
    }
    chazhi(a,n);
    
}
