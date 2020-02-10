/*************************************************************************
	> File Name: 9.2.c
	> Author: 
	> Mail: 
	> Created Time: 2020年02月05日 星期三 11时12分03秒
 ************************************************************************/

#include<stdio.h>
void chline(char ch,int i,int j)
{
    printf("%*c",i,ch);
    for (int k=i;k<j;k++)
    {
        printf("%c",ch);
    }
    printf("\n");
}
int main(void)
{
    char ch;
    int i,j;
    printf("输入字符：");
    scanf("%c",&ch);
    printf("输入起始行：");
    scanf("%d",&i);
    printf("输入终止列：");
    scanf("%d",&j);
    chline(ch,i,j);
}
