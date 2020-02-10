/*************************************************************************
	> File Name: 7.6.c
	> Author: 
	> Mail: 
	> Created Time: 2020年02月02日 星期日 22时46分58秒
 ************************************************************************/

#include<stdio.h>
int main(void)
{
    char present,former;
    int cnt=0;
    while((present=getchar())!='#')
    {
        if(present=='i'&&former=='e')
        {
            cnt++;
        }
        former=present;
    }
    printf("ei出现次数为：%d\n",cnt);
}
