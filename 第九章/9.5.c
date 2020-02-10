/*************************************************************************
	> File Name: 9.5.c
	> Author: 
	> Mail: 
	> Created Time: 2020年02月05日 星期三 11时30分51秒
 ************************************************************************/

#include<stdio.h>
void F(double a,double b)
{
    if(a>b)
        b=a;
    else a=b;
    printf("%lf\t%lf\n",a,b);    
}
int main(void)
{
    double a,b;
    scanf("%lf%lf",&a,&b);
    F(a,b);
}
