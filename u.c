/*************************************************************************
	> File Name: u.c
	> Author: 
	> Mail: 
	> Created Time: 2020年02月10日 星期一 19时24分25秒
 ************************************************************************/

#include<stdio.h>
#include<string.h>
int main(void)
{
    int n,f,k,j,cnt;
    char s[500000];
    while(1)
    {
        f=0;
        k=0;
        j=0;
        cnt=0;
        fgets(s,500000,stdin);
        printf("%d",strlen(s));
        for(int i=0;i<strlen(s)-1;i++)
        
        {
            if(s[i]=='[')
            {
                f=1;
             }
            if(f==1 && j==0)
             {
                cnt++;
             }
            if(f==1 && s[i]==']')
            {
                k=1;
                j=1;
            }
            
        }
        if(f==1 && k==1 && j==1) 
            printf("%d\n",cnt);
        else printf("-1\n");
    }
}
