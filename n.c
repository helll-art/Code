/*************************************************************************
	> File Name: n.c
	> Author: 
	> Mail: 
	> Created Time: 2020年02月09日 星期日 15时09分54秒
 ************************************************************************/

#include<stdio.h>
int main(void)
{
    int n;
    int i,j;
    int a[30][30];
    while(scanf("%d",&n)!=EOF)
    {
        for(i=0;i<n;i++)
        {
            for(j=0;j<=i;j++)
            {
                if(j==0 ||j==i)
                {
                    a[i][j]=1;
                }
                else 
                    a[i][j]=a[i-1][j]+a[i-1][j-1];
            }
        }
        for(i=0;i<n;i++)
        {
            printf("%d",a[i][0]);
            for(j=1;j<=i;j++)
            {
                printf(" %d",a[i][j]);
            }
            printf("\n");
        }
        printf("\n");
    }
}
