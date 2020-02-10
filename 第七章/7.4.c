/*************************************************************************
	> File Name: 7.4.c
	> Author: 
	> Mail: 
	> Created Time: 2020年02月02日 星期日 21时54分02秒
 ************************************************************************/

#include<stdio.h>
int main(void)
{
    char ch;
    while((ch=getchar())!='#')
    {
        if(ch == '.')
            printf("!");
        else if (ch == '!')
            printf("!!");
        else printf("%c",ch);
    }
    printf("\n");
}
