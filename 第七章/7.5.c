/*************************************************************************
	> File Name: 7.5.c
	> Author: 
	> Mail: 
	> Created Time: 2020年02月02日 星期日 22时01分09秒
 ************************************************************************/

#include<stdio.h>
int main(void)
{
    int a,t,osum=0,jsum=0;
    double oaverage,javerage,o=0,j=0;
    while(1)
    {
        scanf("%d",&a);
        if(a==0)  break;
        switch(t=a%2)
        {
            case 1:  jsum=jsum+a;
                     j++;
                     break;
            case 0:  osum=osum+a;
                     o++;
                     break;
            default: break;
        }
    }
    oaverage=osum/o;
    javerage=jsum/j;
    printf("偶数总个数：%.0lf\n偶数平均值：%.1lf\n奇数总个数：%.0lf\n奇数平均值：%.1lf\n",o,oaverage,j,javerage);
}
