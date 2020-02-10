/*************************************************************************
	> File Name: t.c
	> Author: 
	> Mail: 
	> Created Time: 2020年02月10日 星期一 17时26分47秒
 ************************************************************************/

#include<stdio.h>
#define PI 3.1415926
int main(void)
{
    double t1,t2,r;
    double  R,v1,v2;
    while(scanf("%d%d%d",&R,&v1,&v2)!=EOF)
    {
        r=R/v2*v1;
        t1=(R-r)/v1;
        t2=PI*R/v2;
        if(t1<t2)
            printf("YES\n");
        else printf("NO\n");
        
    }
}
