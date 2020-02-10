/*************************************************************************
	> File Name: f.c
	> Author: 
	> Mail: 
	> Created Time: 2020年02月08日 星期六 17时54分25秒
 ************************************************************************/

#include<stdio.h>
#include<string.h>
int main(void)
{
    char ch[100][200];
    int n;
    int s[100];
    int cnt[100]={0};
    scanf("%d",&n);
    for(int i=0;i<n;i++)
    {
        scanf("%s",ch[i]);
        s[i]=strlen(ch[i]);
        for(int j = 0;j<s[i];j++)
        {
            if(ch[i][j]>='0' && ch[i][j]<='9')
            {
                cnt[i]++;
            }
        }
    }
    for(int i=0;i<n;i++)
    {
        printf("%d\n",cnt[i]);
    }
}
