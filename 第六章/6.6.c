/*************************************************************************
	> File Name: 6.6.c
	> Author: 
	> Mail: 
	> Created Time: 2020年01月31日 星期五 22时07分34秒
 ************************************************************************/

#include<stdio.h>
#include<string.h>
int main(void)
{
    char string[100];
    scanf("%s",string);
    int m=strlen(string);
    for (int i=m;i>=0;i--)
    {
        printf("%c",string[i]);
    }
    printf("\n");
}
