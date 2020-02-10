/*************************************************************************
	> File Name: 9.7.c
	> Author: 
	> Mail: 
	> Created Time: 2020年02月05日 星期三 12时01分05秒
 ************************************************************************/

#include<stdio.h>
int Fibonacci(int i)
{
    int k;
    k=0;
    i=i+k;
    Fibonacci(Fibonacci(i));
    printf("%d\t",Fiboncci(i));
    return i;
}
int main(void)
{
    int i=1;
    Fibonacci(i);
}

