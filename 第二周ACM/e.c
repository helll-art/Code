/*************************************************************************
	> File Name: e.c
	> Author: 
	> Mail: 
	> Created Time: 2020年03月14日 星期六 00时20分28秒
 ************************************************************************/
#include<stdio.h>
#include<string.h>
#include<string.h>
int max(int a,int b)
{
        return a>b?a:b; 

}
int dp[100100];
int a[100100],b[100100];
int main()
{
        int i,j;
        int k;
        long long  ans;
        int n,m;
        while (~scanf("%d %d", &n, &m) && n != 0 && m != 0)
        {
             ans=0;
            memset(dp, 0, sizeof(dp));
            for ( i = 1; i <= n; i++ )
                scanf("%d", &a[i]);
            for ( i = 1; i <= n; i++ )
                 scanf("%d", &b[i]);
                 dp[0] = 1;
            for ( i = 1; i <= n; i++ )
            {
                k = 1;
                while (k <= b[i])
                {
                    for ( j = m; j >= k * a[i]; j-- )
                    {
                        dp[j] =max(dp[j - k * a[i]],dp[j]);
                                    
                    }
                    b[i] -= k;
                    k *= 2;                
            }
            if (b[i] == 0)
                continue;
            for ( j = m; j >= b[i] * a[i]; j-- )
            {
                dp[j] =max( dp[j - b[i] * a[i]],dp[j] );
                            
            }
                    
        }
        for ( i = 1; i <= m; i++ )
        {
            ans += dp[i];
                    
        }
        printf("%lld\n",ans);
            
    }
        return 0;

}
