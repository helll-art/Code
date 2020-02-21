/*************************************************************************
	> File Name: c.c
	> Author: 
	> Mail: 
	> Created Time: 2020年02月19日 星期三 21时39分36秒
 ************************************************************************/
/*
 * 要满足给定的重量时  要初始化数组为INF 并且在最后要有判断条件
* */
#include<stdio.h>
 #include<string.h>
 #define INF 0x3f3f3f3f
 int dp[10005];           // 根据存钱罐的重量定长度
 int min(int a ,int b)
 {
     return a<b?a:b;
 }
int main(void)
{
    int t,e,f,n,weight;
    int i,j;
    int p[505],w[505];
    scanf("%d",&t);
    while(t--)
    {
        scanf("%d%d",&e,&f);
        weight=f-e;
        scanf("%d",&n);
        for(i=1;i<=n;i++)
        {
            scanf("%d%d",&p[i],&w[i]);
        }
        memset(dp,INF,sizeof(dp));     //让dp数组所有元素的值为INF
        dp[0]=0;                       //当重量为0时，价值一定为0
        for(i=1;i<=n;i++)
        {
            for(j=w[i];j<=weight;j++)
            {
                dp[j]=min(dp[j],dp[j-w[i]]+p[i]);
            }
            }
        if(dp[weight]!=INF)            //满足恰好为该重量的条件 
            printf("The minimum amount of money in the piggy-bank is %d.\n",dp[weight]);
        else 
            printf("This is impossible.\n");

    }
}
