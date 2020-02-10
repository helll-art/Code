/*************************************************************************
	> File Name: w.c
	> Author: 
	> Mail: 
	> Created Time: 2020年02月10日 星期一 20时34分39秒
 ************************************************************************/

#include<stdio.h>
int main(void)
{
    int n,m,max[100]={0},a[100]={0},sum,temp;
    char ch[100][100];
    int i,j;
    while(scanf("%d%d",&n,&m)!=EOF)
    {
        sum=0;
        for(i=0;i<n;i++)
        {
            for(j=0;j<m;j++)
            {
                top:scanf("%c",&ch[i][j]);
                if(ch[i][j]=='\n')
                    goto top;
            }
        }
        for(j=0;j<m;j++)
        {
            for(i=0;i<n;i++)
            {
                max[j]=ch[i][j]-'0';
                if(max[j]<ch[i][j]-'0')
                {
                    temp=max[j];
                    max[j]=ch[i][j]-'0';
                    ch[i][j]=temp+'0';
                }
            }
        }
        for(j=0;j<m;j++)
        {
            for(i=0;i<n;i++)
            {
                if(max[j]==ch[i][j]-'0')
                {
                    a[i]=1;
                }
                    
                
            }
        }
        for(i=0;i<n;i++)
        {
            if(a[i]!=0)
                sum++;
        }
        printf("%d\n",sum);
    }
}
