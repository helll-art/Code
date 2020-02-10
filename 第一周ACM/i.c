/*************************************************************************
	> File Name: i.c
	> Author: 
	> Mail: 
	> Created Time: 2020年02月08日 星期六 19时29分01秒
 ************************************************************************/

#include<stdio.h>
int main(void)
{
    int n,k,i;
    
    while(scanf("%d%d",&n,&k)!=EOF)
    {
        int a[100]={0};
        int cnt=0,s;
        for(i=0;i<n;i++)
        {
            scanf("%d",&a[i]);
        }
        for(i=0;i<n;i++)
        {
            if(a[i]<=k)
                cnt++;
            else break;
        }
        s=cnt;
        for(i=n-1;i>s-1;i--)
        {
            if(a[i]<=k)
                cnt++;
            else break;
        }
        printf("%d\n",cnt);
    }
}
