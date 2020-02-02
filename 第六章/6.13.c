/*************************************************************************
	> File Name: 6.13.c
	> Author: 
	> Mail: 
	> Created Time: 2020年01月31日 星期五 23时22分38秒
 ************************************************************************/

#include<stdio.h>
int main(void)
{
    double a[8],b[8];
    for (int i=0;i<8;i++)
    {
        scanf("%lf",&a[i]);
    }
    for (int i=1;i<8;i++)
    {
        b[0]=a[0];
        for (int j=1;j<=i;j++)
        {
            b[i]=a[j]+b[i-1];
        }
    }
    printf("数组a：");
    for (int i=0;i<8;i++)
    {
        printf("%.1lf\t",a[i]);
    }
    printf("\n数组b：");
    for (int i=0;i<8;i++)
    {
        printf("%.1lf\t",b[i]);
    }
    printf("\n");
}
