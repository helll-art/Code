/*************************************************************************
	> File Name: 2.2.c
	> Author: 
	> Mail: 
	> Created Time: 2020年01月28日 星期二 12时00分40秒
 ************************************************************************/

#include<stdio.h>
int main()
{
    char name[50];
    char address[50];
    printf("请输入您的姓名：");
    scanf("%s",name);
    printf("请输入您的地址:");
    scanf("%s",address);
    printf("您的姓名是：%s\n您的地址是：%s\n",name,address);
    return 0;
}
