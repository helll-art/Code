/*************************************************************************
	> File Name: 9.3.c
	> Author: 
	> Mail: 
	> Created Time: 2020年02月05日 星期三 11时21分51秒
 ************************************************************************/

#include<stdio.h>
void F(char ch,int i,int j)
{
    for(int k=0;k<i;k++)
        printf("\n");
    for(int k=0;k<j;k++)
        printf("%c\n",ch);
}
int main(void)
{
    char ch;
    int i,j;
    printf("输入字符：");
    scanf("%c",&ch);
    printf("输入起始行：");
    scanf("%d",&i);
    printf("输入总行数：");
    scanf("%d",&j);
    F(ch,i,j);
}
