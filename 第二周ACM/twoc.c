/*************************************************************************
	> File Name: twoc.c
	> Author: 
	> Mail: 
	> Created Time: 2020年02月20日 星期四 23时21分56秒
 ************************************************************************/

#include<stdio.h>
#include<string.h>
#define INF 0x3f3f3f3f
int dp[100010];
int min(int a,int b)
{
    return a<=b?a:b;
}
int main(void)
{
    int T,E,F,n;
    int p[505]={0},w[505]={0};
    scanf("%d",&T);
    while(T--)
    {
        memset(dp,INF,sizeof(dp));
        dp[0]=0;
        scanf("%d%d",&E,&F);
        scanf("%d",&n);
        int weight=F-E;
        for (int i=1;i<=n;i++)
            scanf("%d%d",&p[i],&w[i]);
        for(int i=1;i<=n;i++)
        {
            for (int j=1;j<=weight;j--)
            {
                for (int k=0;k<=j/w[i];k++)
                {
                    dp[j]=min(dp[j],dp[j-k*w[i]]+p[i]);
                }
            }
        }
        if(dp[weight]!=INF)
            printf("The minimum amount of money in the piggy-bank is %d.\n",dp[weight]);
        else 
            printf("This is impossible.\n");

    }
}
