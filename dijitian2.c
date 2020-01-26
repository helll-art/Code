/*************************************************************************
	> File Name: dijitian2.c
	> Author: 
	> Mail: 
	> Created Time: 2019年12月09日 星期一 13时44分54秒
 ************************************************************************/

#include<stdio.h>
int main()
{
    int i,j;
    int nian,yue,ri;
    int a[13]={0,31,28,31,30,31,30,31,31,30,31,30,31};
    while(scanf("%d/%d/%d",&nian,&yue,&ri)!=EOF)
    {
        int sum=0,y=0;
        if(nian%4==0&&nian%100!=0||nian%400==0)
        a[2]=29;
        else {
            a[2]=28;
        }
        for(i=0;i<yue;i++)
        {
            sum=sum+a[i];
        }
        y=sum+ri;
        printf("%d\n",y);
    }
    return 0;
}
