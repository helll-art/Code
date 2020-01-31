/*************************************************************************
	> File Name: 4.4.c
	> Author: 
	> Mail: 
	> Created Time: 2020年01月30日 星期四 11时23分29秒
 ************************************************************************/

#include<stdio.h>
int main()
{
    char name[40];
    double Long;
    scanf("%s%lf",name,&Long);
    printf("%s, you are %.3lf feet tall.\n",name,Long);
    return 0;
}
