/*************************************************************************
	> File Name: 5.5.c
	> Author: 
	> Mail: 
	> Created Time: 2020年01月31日 星期五 12时55分33秒
 ************************************************************************/

#include<stdio.h>
int main(void)
{
    int cnt;
    int sum=0,a=1;
    scanf("%d",&cnt);
    while(cnt>0)
    {
        sum=sum+a;
        a++;
        cnt--;
    }
    printf("%d",sum);
}
