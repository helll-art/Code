/*************************************************************************
	> File Name: 7.2.c
	> Author: 
	> Mail: 
	> Created Time: 2020年02月01日 星期六 23时44分14秒
 ************************************************************************/

#include<stdio.h>
int main(void)
{
    char ch;
    int cnt=1;
    while((ch=getchar())!='#')
    {
        printf("%c/%d\t",ch,ch);
        if(cnt % 8 == 0 )
            printf("\n");
        cnt++;
    }
    printf("\n");
}
