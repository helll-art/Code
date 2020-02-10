/*************************************************************************
	> File Name: 9.4.c
	> Author: 
	> Mail: 
	> Created Time: 2020年02月05日 星期三 11时26分52秒
 ************************************************************************/

#include<stdio.h>
void F(double a,double b)
{
    double c;
    c=(1/a+1/b)/2;
    printf("谐均值：%lf\n",1/c);
}
int main(void)
{
    double a,b;
    printf("输入两数，求其谐均值：");
    scanf("%lf%lf",&a,&b);
    F(a,b);
}
