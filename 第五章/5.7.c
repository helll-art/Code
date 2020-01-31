/*************************************************************************
	> File Name: 5.7.c
	> Author: 
	> Mail: 
	> Created Time: 2020年01月31日 星期五 13时21分27秒
 ************************************************************************/

#include<stdio.h>
int main(void)
{
    int cnt;
    int sum=0;
    printf("输入要结束的位置：");
    scanf("%d",&cnt);
    while(cnt>0)
    {
        sum=sum+cnt*cnt;
        cnt--;
    }
    printf("%d\n",sum);
}
