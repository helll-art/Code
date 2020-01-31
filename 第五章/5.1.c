/*************************************************************************
	> File Name: 5.1.c
	> Author: 
	> Mail: 
	> Created Time: 2020年01月31日 星期五 11时12分37秒
 ************************************************************************/
 //while(1)保证次次可以循环 ，在程序里结束程序(return 0;)
 #include<stdio.h>
#define TIME 60
int main(void)
{
    int time;
    while(1)
    {
        printf("请输入转化数据：");
        scanf("%d",&time);
        if (time > 0)
        {
            printf("%d小时%d分钟\n",time/TIME,time%TIME);
        }
        else
        {
            printf("数据异常，退出程序。\n");
            return 0;
        }
    }
}
