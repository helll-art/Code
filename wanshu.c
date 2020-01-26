/*************************************************************************
	> File Name: wanshu.c
	> Author: 
	> Mail: 
	> Created Time: 2019年12月02日 星期一 10时41分34秒
 ************************************************************************/

#include<stdio.h>
int main()
{
    int n,i,j,k;
    int num1[20],num2[20];
    int cnt[20]={0},sum=0;
    printf("请输入行数：");
    scanf("%d",&n);
    for(i=0;i<n;i++)
    {
        scanf("%d%d",&num1[i],&num2[i]);
    }
    for(i=0;i<n;i++)
    {
        sum=0;
        for(j=num1[i];j<=num2[i];j++)
        {
            for(k=1;k<=j;k++)
            {
                if(j%k==0){
                sum=sum+j;
                printf("%d\n",sum);
             }
            if(sum==j){
            cnt[i]++;
            
                }
             }
        }    
    for(i=0;i<n;i++)
    {
        printf("%d\n",cnt[i]);
    }
    return 0;
}
