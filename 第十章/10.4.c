/*************************************************************************
	> File Name: 10.4.c
	> Author: 
	> Mail: 
	> Created Time: 2020年02月07日 星期五 20时22分54秒
 ************************************************************************/

#include<stdio.h>
int suoyin(double a[],int n)
{
    int i;
    int  max=0;
    for(i=1;i<n;i++)
    {
        if(a[max] < a[i])
            max =  i ;
    }
    return max;
}
int main(void)
{
    double a[5]={0.4,5.5,6,4.3,10.3};
    printf("最大数索引为%d\n",suoyin(a,5));
}
