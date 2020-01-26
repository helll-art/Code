/*************************************************************************
	> File Name: dashujiafa.c
	> Author: 
	> Mail: 
	> Created Time: 2019年12月12日 星期四 21时13分11秒
 ************************************************************************/

#include<stdio.h>
#include<string.h>
int main()
{
    int t,x,y;
    char a[10000]="0",b[10000]="0",c[10000]="0";
    while(scanf("%s %s",a,b)!=EOF)
    
    {
        int sum;
        x=strlen(a);
        y=strlen(b);
        if(x>y) t=x;
        else t=y;
        for(t;t>0;t--)
        {
            sum=a[t]+b[t]-48;
            if(sum>'9')
            {
                c[t]=a[t]+b[t]-48+1;
            }
            else c[t]=a[t]+b[t]-48;
        }
        for(int i=0;i<t;i++)
        {
            printf("%c",c[i]);
        }
        
        printf("\n");

    }
    return 0;
}
