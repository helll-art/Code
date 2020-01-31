/*************************************************************************
	> File Name: 4.2.c
	> Author: 
	> Mail: 
	> Created Time: 2020年01月30日 星期四 11时04分48秒
 ************************************************************************/

#include<stdio.h>
#include<string.h>
int main()
{
    char name[40];
    
    scanf("%s",name);
    const int a=strlen(name);
    printf("\"%s\"\n",name);
    printf("\"%20s\"\n",name);
    printf("\"   %s\"\n",name);
    return 0;
}
