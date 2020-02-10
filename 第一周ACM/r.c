/*************************************************************************
	> File Name: r.c
	> Author: 
	> Mail: 
	> Created Time: 2020年02月09日 星期日 18时02分50秒
***********************************************************************/
#include<stdio.h>
#include<string.h>
int main(void)
{
    char ch[100][100];
    int num1[100]={0},num2[100]={0},num3[100]={0},num4[100]={0},num5[100]={0};
    int n,i;
    int len[100]={0};
    scanf("%d",&n);
    for(i=0;i<=n;i++)
    {
        fgets(ch[i],100,stdin);
    }
    int cnt=1;
    while(cnt<=n)
    {
        for(i=0;i<strlen(ch[cnt]);i++)
        {
            if(ch[cnt][i]=='a')
                num1[cnt]++;
            else if (ch[cnt][i]=='e')
                num2[cnt]++;
            else if(ch[cnt][i]=='i')
                num3[cnt]++;
            else if (ch[cnt][i]=='o')
                num4[cnt]++;
            else if (ch[cnt][i]=='u')
                num5[cnt]++;
        }
        cnt++;
    }
    for(i=1;i<=n;i++)
    {
        printf("a:%d \ne:%d \ni:%d \no:%d \nu:%d",num1[i],num2[i],num3[i],num4[i],num5[i]);
        if(i!=n)
            printf(" \n\n");
        else printf("\n");
    }

}
