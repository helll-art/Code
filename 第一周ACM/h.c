/*************************************************************************
	> File Name: h.c
	> Author: 
	> Mail: 
	> Created Time: 2020年02月08日 星期六 19时01分50秒
 ************************************************************************/
 #include<stdio.h>
 int main(void)
 {
     int a[1000][3]={0};
     int cnt,n,i,j;
     while(scanf("%d",&n)!=EOF)
     {
         cnt=0;
         int sum[1000]={0};
         for(i=0;i<n;i++)
         {
             for(j=0;j<3;j++)
             {
                 scanf("%d",&a[i][j]);
                 sum[i]=sum[i]+a[i][j];
             }
             if(sum[i]>1)
             {
                cnt++;
             }
         }
         printf("%d\n",cnt);
     }

 }
