/*************************************************************************
	> File Name: A.c
	> Author: 
	> Mail: 
	> Created Time: 2020年02月08日 星期六 17时07分31秒
 ************************************************************************/

#include<stdio.h>
int main(void)
{
    int n,m;
    int i=0,j;
    int a[100][100];
    while(1)
    {
        int sum[100]={0};
        scanf("%d",&n);
        if(n!=0)
        {
            scanf("%d",&m);
            for(i=0;i<n;i++)
            {
                for(j=0;j<m;j++)
                {
                    scanf("%d",&a[i][j]);
                    sum[i]=sum[i]+a[i][j];
                }
            }
            for(i=0;i<n;i++)
            {
                printf("%d\n",sum[i]);
            }
        }
        else return 0;
    }
}
