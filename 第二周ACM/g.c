/*************************************************************************
	> File Name: g.c
	> Author: 
	> Mail: 
	> Created Time: 2020年03月17日 星期二 22时45分57秒
 ************************************************************************/
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
char b[100000][210];
int main(void)
{
    char ch;
    int count = 0;
    int j=0;
    //不能让回车代表输入终止，使用ctrl+d
    while((ch=getchar())!=EOF)    //分割出每个单词
    {
        if(isalpha(ch))
        {
            b[count][j++] = tolower(ch);
        }
        else if(isalpha(b[count][0]))
        {
            b[count++][j]='\0';
            j=0;
        }
    }
    char temp[210];
    for(int i=0;i<count;i++) //对每个单词进行排序
    {
        for(int j=i;j<count;j++)
        {
            if(strcmp(b[i],b[j])>0)
            {
                strcpy(temp,b[i]);
                strcpy(b[i],b[j]);
                strcpy(b[j],temp);
            }
        }
    }
    for(j=0;j<count;j++)
    {
        if(strcmp(b[j],b[j+1]))   //去重
            printf("%s\n",b[j]);
    }

}
