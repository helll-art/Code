/*************************************************************************
	> File Name: 4.3.c
	> Author: 
	> Mail: 
	> Created Time: 2020年01月30日 星期四 11时18分09秒
 ************************************************************************/

#include<stdio.h>
int main()
{
    double a;
    scanf("%lf",&a);
    printf("a.The input is %.1lf or %.1e\n",a,a);
    printf("b.The input is %+lf or %.3e\n",a,a);
    return 0;
}
