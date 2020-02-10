/*************************************************************************
	> File Name: 7.7.c
	> Author: 
	> Mail: 
	> Created Time: 2020年02月02日 星期日 23时16分27秒
 ************************************************************************/

#include<stdio.h>
#define RATE1 0.15
#define RATE2 0.2
#define RATE3 0.25
#define TIME 1.5
int main(void)
{
    double time,money,jgz,sj;
    scanf("%lf",&time);
    if(time>40)
        time=40+(time-40)*TIME;
    money=time*10;
    if(money<300)
        sj=money*RATE1;
    else if(money<450)
        sj=300*RATE1+(money-300)*RATE2;
    else sj=300*RATE1+150*RATE2+(money-450)*RATE3;
    printf("*************************************\n");
    printf("工作时间：%.1lf\n工资总额：%lf\n税金：%lf\n净工资：%lf\n",time,money,sj,money-sj);
    printf("*************************************\n");

}
