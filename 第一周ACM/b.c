/*************************************************************************
	> File Name: b.c
	> Author: 
	> Mail: 
	> Created Time: 2020年02月08日 星期六 17时24分50秒
 ************************************************************************/

 #include<stdio.h>
 int main(void)
 {
     int m,n;
     int i,j;
     int sum[100]={0};
     int a[100][100];
     scanf("%d",&m);       
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
    }
}
