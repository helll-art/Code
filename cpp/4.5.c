/*************************************************************************
	> File Name: 4.5.c
	> Author: 
	> Mail: 
	> Created Time: 2020年01月30日 星期四 11时49分39秒
 ************************************************************************/

#include<stdio.h>
#include<string.h>
int main()
{
    char name1[40];
    char name2[40];
    scanf("%s%s",name1,name2);
    printf("%s %s\n",name1,name2);
    printf("%*d %*d\n",strlen(name1),strlen(name1),strlen(name2),strlen(name2));
    return 0;

}
