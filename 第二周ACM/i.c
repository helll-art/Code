/*************************************************************************
	> File Name: i.c
	> Author: 
	> Mail: 
	> Created Time: 2020年03月14日 星期六 22时16分44秒
 ************************************************************************/

#include<stdio.h>
int child[120];
int main(void)
{
    int n,k,x;
    int i,j;
    scanf("%d%d",&n,&k);
    int cnt,first=1,ans;
    for(j=0;j<k;j++)
    {
        scanf("%d",&x);
        cnt=x%(n-j);      //因为x的范围过大，所以取余优化
        if(cnt==0)
            cnt=n-j;      //如果淘汰的是自己，让最后一个人当领导
        for(i=first+1; ; i++)
        {
            if(i>n)
                i=1;
            if(child[i]==0)
                cnt--;
            if(cnt==0)
            {
                child[i]=1;
                ans=i;
                for(int l=i+1; ; l++)     //筛选已经被淘汰的人
                {
                    if(l>n)
                        l=1;
                    if(child[l]==0)
                    {
                        first=l;
                        break;  //break 针对的是循环体或switch 所以不需要对外层for继续break
                    }
                }
                break;
            }
        }
        printf("%d ",ans);
    }
}
