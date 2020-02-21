/*************************************************************************
	> File Name: e.c
	> Author: 
	> Mail: 
	> Created Time: 2020年02月20日 星期四 22时47分43秒
 ************************************************************************/

#include<stdio.h>
int max(int a,int b)
{
    return a>b?a:b;
}
int dp[100];
int main(void)
{
    int s,n;
    int w[100]={0},v[100]={0},m[100]={0};
    scanf("%d%d",&n,&s);
    for(int i=1;i<=n;i++)
    {
        scanf("%d%d%d",&w[i],&v[i],&m[i]);
    }
    for(int i=1;i<=n;i++)
    {


            for( int j=w[i];j<=s;j++)
            {
                for(int k=0;k<=m[i];k++)
                {
                    if(j>=m[i]*w[i])         //该条件不能漏  否则会超过背包容量
                        dp[j]=max(dp[j],dp[j-k*w[i]]+k*v[i]);
                }
            }
    }
    for(int j=0;j<s ;j++)
        printf("%d\n",dp[s]);
}
