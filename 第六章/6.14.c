/*************************************************************************
	> File Name: 6.14.c
	> Author: 
	> Mail: 
	> Created Time: 2020年01月31日 星期五 23时43分33秒
 ************************************************************************/

#include<stdio.h>
int main(void)
{
    char a[255];
    for (int i=0;i<20;i++)
        scanf("%c",&a[i]);
    for(int i=19;i>=0;i--)
        printf("%c",a[i]);
    printf("\n");
    return 0;
}
