/*************************************************************************
	> File Name: 7.10.c
	> Author: 
	> Mail: 
	> Created Time: 2020年02月03日 星期一 00时19分39秒
 ************************************************************************/

#include<stdio.h>
int main(void)
{
    int a;
    int max;
    double sj,shouru;
    while(1){
    printf("**********************************************************\n");
    printf("1）：单身\t\t 前$17850按15%，超出部分按28%\n");
    printf("2）：户主\t\t 前$23900按15%，超出部分按28%\n");
    printf("3）：已婚，共有\t\t 前￥29750按15%，超出部分按28%\n");
    printf("4）：已婚，离异\t\t 前$14875按15%，超出部分按28%\n");
    printf("**********************************************************\n");
    printf("请输入用户类型和应征税收入：");
    scanf("%d%lf",&a,&shouru);
    switch(a)
        {
            case 1:max=17850;break;
            case 2:max=23900;break;
            case 3:max=29750;break;
            case 4:max=14875;break;
            default:break;
        }
    if (shouru<max)
        sj=shouru*0.15;
    else sj=max*0.15+(shouru-max)*0.28;
    printf("税金为%.1lf\n",sj);
    }
}
