/*************************************************************************
	> File Name: 7.8.c
	> Author: 
	> Mail: 
	> Created Time: 2020年02月02日 星期日 23时43分47秒
 ************************************************************************/

#include<stdio.h>
#define RATE1 0.15
#define RATE2 0.2
#define RATE3 0.25
#define TIME 1.5
int main(void)
{
    int a;
    double shixin,time,money,sj;
    printf("******************************************************************\n");
    printf("Enter the number corresponding to the desired pay rate or action:\n");
    printf("1)$8.75/hr\t\t\t\t\t2)$9.33/hr\n");
    printf("3)10.00/hr\t\t\t\t\t4)11.20/hr\n");
    printf("5)quit\n");
    printf("******************************************************************\n");
    top: scanf("%d",&a);
    switch(a)
    {
        case 1:shixin=8.75;break;
        case 2:shixin=9.33;break;
        case 3:shixin=10.00;break;
        case 4:shixin=11.2;break;
        case 5:return 0;
        default:printf("请输入正确选项！");goto top;break;
    }
    printf("请输入工作小时数：");
    scanf("%lf",&time);
    if(time>40)
        time=TIME*(time-40)+40;
    money=time*shixin;
    if(money<300)
        sj=money*RATE1;
    else if(money<450)
        sj=300*RATE1+(money-300)*RATE2;
    else sj=300*RATE1+150*RATE2+(money-450)*RATE3;
    printf("工作时间：%.1lf\n工作金额：%lf\n税金：%lf\n净工资：%lf\n",time,money,sj,money-sj);
}
