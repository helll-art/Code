/*************************************************************************
	> File Name: s.c
	> Author: 
	> Mail: 
	> Created Time: 2020年02月10日 星期一 16时47分49秒
 ************************************************************************/

#include<stdio.h>
int main(void)
{
    int n;
    while(scanf("%d",&n)!=EOF)
    {

    int a[1000]={0};
    int i,j,temp;
    for( i=0;i<n;i++)
    {
        scanf("%d",&a[i]);
    }
    for(i=0;i<n;i++)
    {
        for(j=i;j<n;j++)
        {
            if(a[i]>=a[j])
            {
                temp=a[i];
                a[i]=a[j];
                a[j]=temp;
            }
        }
    }

    if(n%2==0)
    {
        printf("%d\n",a[n/2-1]);
    }
    else printf("%d\n",a[n/2]);
    }
}

