/*************************************************************************
	> File Name: quanmiankaolv.c
	> Author: 
	> Mail: 
	> Created Time: 2019年12月09日 星期一 15时22分58秒
 ************************************************************************/

#include<stdio.h>
int main()
{
    double a,b;
    while(scanf("%lf%lf",&a,&b)!=EOF)
    {
        double x,y;
        x=a*100000;
        y=b*100000;
        if(a==b)
        printf("YES\n");
        else 
        printf("NO\n");
    }
    return 0;
}
