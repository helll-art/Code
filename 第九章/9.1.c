/*************************************************************************
	> File Name: 9.1.c
	> Author: 
	> Mail: 
	> Created Time: 2020年02月05日 星期三 11时09分00秒
 ************************************************************************/

#include<stdio.h>
double min(double x,double y)
{
    return (x>y?y:x);
}
int main(void)
{
    double a,b;
    scanf("%lf%lf",&a,&b);
    printf("%lf\n",min(a,b));

}
