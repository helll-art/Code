/*************************************************************************
	> File Name: v.c
	> Author: 
	> Mail: 
	> Created Time: 2020年02月10日 星期一 20时18分30秒
 ************************************************************************/

#include<stdio.h>
int main(void)
{
    int n,i,j,temp;
    int a[5];
    while(scanf("%d",&n)!=EOF)
    {
        for( i=0;i<n;i++)
        {
            scanf("%d",&a[i]);
        }
        for(i=0;i<n;i++)
        {
            for(j=i;j<n;j++)
            {
                if(a[i]>a[j])
                {
                    temp=a[i];
                    a[i]=a[j];
                    a[j]=temp;
                }
            }
        }
        printf("%d\n",a[n/2]);
    }
}
