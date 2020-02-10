/*************************************************************************
	> File Name: 9.6.c
	> Author: 
	> Mail: 
	> Created Time: 2020年02月05日 星期三 11时38分04秒
 ************************************************************************/

#include<stdio.h>
int panduan(char ch)
{
    if(ch >= 'A' && ch <= 'z')
        return 1;
    else return 0;
}
int main(void)
{
    int i;
    char ch;
    while(scanf("%c",&ch)!=EOF)
    {
        i=ch-'A';
        if(panduan(ch) && i<=26)
            printf("%c在%d位置。\n",ch,i+1);
        else if (panduan(ch) && i>26)
        printf("%c在%d位置。\n",ch,i-31);
    }
}
