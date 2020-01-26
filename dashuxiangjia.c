/*************************************************************************
	> File Name: dashuxiangjia.c
	> Author: 
	> Mail: 
	> Created Time: 2019年12月12日 星期四 22时06分42秒
 ************************************************************************/
#include<stdio.h>
#include<string.h>
int main()
{
    char c[10000]="0", a[10000]="0",b[10000]="0";
    scanf("%s%s",a,b);
    int x=strlen(a);
    int y=strlen(b);
    int t=x>y?x:y;
    int k=t;
    int i;
    for(i=0;i<t;i++)
    {
        c[k--]=a[i];
    }
    c[k]='\0';
    printf("%s\n",c);
    return 0;
    
}
