/*************************************************************************
	> File Name: yanghuisanjiao.c
	> Author: 
	> Mail: 
	> Created Time: 2019年12月09日 星期一 11时36分05秒
 ************************************************************************/
#include<stdio.h>
int main()
{
    int a[30][30]={0};
    int i,j,f=0;
    int n;
    while(scanf("%d",&n)!=EOF){
    for(i=0;i<n;i++){

        for(j=0;j<n;j++)
        {
            if(j==0||j==i)
                a[i][j]=1;
            else if(i>j)
                a[i][j]=a[i-1][j]+a[i-1][j-1];
        }
    }
    for(i=0;i<n;i++)
    {
        for(j=0;j<n;j++)
        {
            if(i>=j)
            {
                if(i>j)
                printf("%d ",a[i][j]);
                else
                printf("%d",a[i][j]);
            }
            
            
        }
        printf("\n");
    }
    printf("\n");
    }       
    return 0;
}
