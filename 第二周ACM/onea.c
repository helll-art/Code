/*************************************************************************
	> File Name: onea.c
	> Author: 
	> Mail: 
	> Created Time: 2020年02月19日 星期三 21时39分57秒
    01背包问题一维做法
 ************************************************************************/

/* 对于循环的解释：
 * 无论结果如何 都是从f[i][j]=f[i-1][j] 或 
 *                    f[i][j]=f[i-1][j-w[i]]+v[i]中做选择
 *              因为每次的结果都是向前退一行，i并不会影响到结果  所以可以去掉i
 *但应该注意j循环的条件有变化      j从w[i]开始会影响结果
 */
#include<stdio.h>
int f[1010];
int main(void)
{
    int t,n,s;
    int i,j;
    int value1,value2;
    int w[1010]={0},v[1010]={0};
    scanf("%d",&t);

    while(t--)
    {
        scanf("%d%d",&n,&s);
        for(i=1;i<=n;i++)
            scanf("%d",&v[i]);
        for(i=1;i<=n;i++)
            scanf("%d",&w[i]);
        for(i=1;i<=n;i++)
        {
            for(j=s;j>=w[i];j--)
            {
                value1=f[j];
                value2=f[j-w[i]]+v[i];
                f[j]=value1>=value2?value1:value2;
            }
        }
        printf("%d\n",f[s]);

    }
}

