/*************************************************************************
	> File Name: a.c
	> Author: 
	> Mail: 
	> Created Time: 2020年02月19日 星期三 21时17分18秒
    01背包问题二维做法
 ************************************************************************/
/*
 * 如果是要求恰好装满背包，那么在初始化时除了f[0] f[0]f[0]为0 00其它f[1...V] f[1...V]f[1...V]均设为−∞ -∞−∞，这样就可以保证最终得到的f[N] f[N]f[N]是一种恰好装满背包的最优解。
如果并没有要求必须把背包装满，而是只希望价格尽量大，初始化时应该将f[0...V] f[0...V]f[0...V]全部设为0 00。
为什么呢？可以这样理解：初始化的f ff数组事实上就是在没有任何物品可以放入背包时的合法状态。如果要求背包恰好装满，那么此时只有容量为0 00的背包可能被价值为0 00的nothing nothingnothing“恰好装满”，其它容量的背包均没有合法的解，属于未定义的状态，它们的值就都应该是−∞ -∞−∞了。如果背包并非必须被装满，那么任何容量的背包都有一个合法解“什么都不装”，这个解的价值为0 00，所以初始时状态的值也就全部为0 00了。这个小技巧完全可以推广到其它类型的背包问题，后面也就不再对进行状态转移之前的初始化进行讲解
*/
#include<stdio.h>
int f[1010][1010];            // 全局变量未赋值 自动初始化为0
int main(void)
{
    int t,n,s;                
    int value1,value2;
    int v[1000]={0},w[1000]={0};
    scanf("%d",&t);
    while(t--)                //输入场数t，t降为0自动退出循环
    {
        scanf("%d%d",&n,&s);
        for(int i=1;i<=n;i++)
        {
            scanf("%d",&v[i]);
        }
        for(int i=1;i<=n;i++)
        {
            scanf("%d",&w[i]);
        }
        for(int i=1;i<=n;i++)                      //两层循环  i为物品的件数   j未背包的容量
        {
            for(int j=1;j<=s;j++)
            {
                if(j>=w[i])                         //因为此时背包的容量大于第i件物品的重量  所以你可以选择偷或不偷
                {
                    value1=f[i-1][j-w[i]]+v[i];      //  偷
                    value2=f[i-1][j];               //   不偷
                    f[i][j]=value1>=value2?value1:value2;
                }
                else f[i][j]=f[i-1][j];             // 此时背包装不下第i物品
            }
        }
        printf("%d\n",f[n][s]); 
    }
}
