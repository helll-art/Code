/*************************************************************************
	> File Name: e.c
	> Author: 
	> Mail: 
	> Created Time: 2020年02月08日 星期六 17时44分09秒
 ************************************************************************/
#include<stdio.h>
int main(void)
{
    int a[100][100];
    int m,n;
    int i,j;
    int sum[100]={0};
    scanf("%d",&m); //m代表行数
    for(i=0;i<m;i++)
    {
        scanf("%d",&n);
        for(j=0;j<n;j++)
        {
            scanf("%d",&a[i][j]);
            sum[i]=sum[i]+a[i][j];
        }
        
    }
    for(i=0;i<m;i++)
    {
        printf("%d\n",sum[i]);
        if(i!=m-1)
            printf("\n");
    }
}
