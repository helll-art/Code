/*************************************************************************
	> File Name: j.c
	> Author: 
	> Mail: 
	> Created Time: 2020年02月08日 星期六 22时18分06秒
 ************************************************************************/

#include<stdio.h>
int main(void)
{
    int a[1000]={0},b[1000]={0},c[1000]={0}; // a为下车人数，b为上车人数 c为差值
    int sum=0,n,max,i;
    scanf("%d",&n);
    for( i=0;i<n;i++)
    {
        scanf("%d%d",&a[i],&b[i]);
    }
    c[0]=b[0]-a[0];
    for( i=1;i<n;i++)
    {
        c[i]=c[i-1]-a[i]+b[i];
    }
    for( i=0,max=c[0];i<n;i++)
    {
        if(max<c[i])
            max=c[i];
    }
    printf("%d\n",max);

}
