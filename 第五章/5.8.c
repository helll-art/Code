/*************************************************************************
	> File Name: 5.8.c
	> Author: 
	> Mail: 
	> Created Time: 2020年01月31日 星期五 13时30分29秒
 ************************************************************************/

#include<stdio.h>
void Temperatures(double a )
{
    const double cl1 = 1.8, cl2 = 32.0, cl3 = 273.16;
    printf("Fathrenheit = %.2lf\nCelsius = %.2lf\nKelvin = %.2lf\n",a,a*cl1+cl2,a+cl3);
}
int main(void)
{
    double Fathrenheit;
    printf("请输入华氏温度，若要退出程序请输入'q'或非数字值：");
    while(scanf("%lf",&Fathrenheit)==1)
    {
        Temperatures(Fathrenheit);
        printf("请输入华氏温度，若要退出程序请输入'q'或非数字值：");
    }
}
