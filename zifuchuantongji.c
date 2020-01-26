#include<stdio.h>
#include<string.h>
int main()
{
    int i,j,k=0,n,x;
    char ch[80][80];
    int cnt[80]={0};
    printf("请输入你想要的行数：");
    scanf("%d",&n);
    for(i=0;i<n;i++)
    {
        scanf("%s",&ch[i]);
        x=strlen(ch[i]);
        for(j=0;j<x;j++)
        {
            if(ch[i][j]>='0'&&ch[i][j]<='9')
            cnt[i]++;
        }
    }
    for(i=0;i<n;i++)
        printf("%d\n",cnt[i]);
    return 0;
}
