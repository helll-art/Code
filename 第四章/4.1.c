/*************************************************************************
	> File Name: 4.1.c
	> Author: 
	> Mail: 
	> Created Time: 2020年01月30日 星期四 10时40分35秒
 ************************************************************************/
//scanf 不能读取带有空格的内容 可以用两个scanf读取
#include<stdio.h>
int main()
{
    char xing[40];
    char shi[40];
    scanf("%s%s",xing,shi);
    printf("%s %s,%s\n",xing,shi,xing);
    return 0;
}
