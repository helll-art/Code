/*************************************************************************
	> File Name: g.c
	> Author: 
	> Mail: 
	> Created Time: 2020年02月08日 星期六 18时37分45秒
 ************************************************************************/

#include<stdio.h>
int main(void)
{
    int d1,d2,d3,i;
    int sum[4],min;
    while(scanf("%d%d%d",&d1,&d2,&d3)!=EOF)
    {
        if(d1>0 && d2>0 && d3>0)
        {
        sum[0]=d1+d2+d3;
        sum[1]=2*(d1+d2);
        sum[2]=2*(d2+d3);
        sum[3]=2*(d1+d3);
            for( i=0,min=sum[0];i<4;i++)
            {
                if(min>=sum[i])
                {
                    min=sum[i];
                }
            }
            printf("%d\n",min);

        }
        else return 0;
    }
}
