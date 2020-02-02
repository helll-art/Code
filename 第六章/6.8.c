/*************************************************************************
	> File Name: 6.8.c
	> Author: 
	> Mail: 
	> Created Time: 2020年01月31日 星期五 22时43分19秒
 ************************************************************************/

#include<stdio.h>
void F(double a,double b)
{
    printf("( %.1lf - %.1lf ) / %.1lf * %.1lf = %.1lf",a,b,a,b,(a-b)/a*b);
}
int main(void)
{
    double a,b;
    printf("input:");
    while(scanf("%lf%lf",&a,&b) == 2)
    {
        F(a,b);
        printf("\ninput:");
    }
    printf("end\n");
}
