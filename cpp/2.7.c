/*************************************************************************
	> File Name: 2.7.c
	> Author: 
	> Mail: 
	> Created Time: 2020年01月29日 星期三 21时24分58秒
 ************************************************************************/

#include<stdio.h>
void two(void);
void one_three(void);
int main()
{
    printf("starting now:\n");
    one_three();
    printf("done!\n");
    return 0;
}
void two(void)
{
    printf("two\n");
}
void one_three(void)
{
    printf("one\n");
    two();
    printf("three\n");
}
