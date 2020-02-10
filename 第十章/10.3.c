/*************************************************************************
	> File Name: 10.3.c
	> Author: 
	> Mail: 
	> Created Time: 2020年02月07日 星期五 19时51分54秒
 ************************************************************************/

#include<stdio.h>
int max(int [],int);
int main(void)
{
    int a[5]={ 1,2,3,4,5 };
    printf("最大的数为：%d\n",max( a,5 ));
}
int max(int a[],int n)
{
    int max,i;
    for ( i = 0, max = a[0];i < n;i++)
        if(a[i] >= max)
            max = a[i];
    return max;
}
