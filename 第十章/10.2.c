/*************************************************************************
	> File Name: 10.2.c
	> Author: 
	> Mail: 
	> Created Time: 2020年02月07日 星期五 19时42分49秒
 ************************************************************************/

#include<stdio.h>
void copy_arr(double a[],double b[],int n)
{
    printf("用数组符号表示：");
    for (int i=0;i<n;i++)
    {
        b[i]=a[i];
        printf("%.2lf\t",b[i]);
    }
    printf("\n");
}
void copy_ptr(double a[],double b[],int n)
{
    printf("用指针表示：");
    for (int i=0;i<n;i++)
    {
        b[i]=*(a+i);
        printf("%.2lf\t",b[i]);
    }
    printf("\n");
}
int main(void)
{
    double source[5]={1.1,2.2,3.3,4.4,5.5};
    double target1[5];
    double target2[5];
    copy_arr(source,target1,5);
    copy_ptr(source,target2,5);
}
