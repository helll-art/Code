/*************************************************************************
	> File Name: mayarili.c
	> Author: 
	> Mail: 
	> Created Time: 2019年12月16日 星期一 11时39分26秒
 ************************************************************************/

#include<stdio.h>
#include<string.h>

int main()
{
        int i,y[20],d[20],j,sum=0;
        int n;
        char haab[20][20]={"pop", "no", "zip", "zotz", "tzec", "xul", "yoxkin", "mol", "chen", "yax", "zac", "ceh", "mac", "kankin", "muan", "pax", "koyab", "cumhu", "uayet"};
        char tzolkin[20][20]={"imix", "ik", "akbal", "kan", "chicchan", "cimi", "manik", "lamat", "muluk", "ok",  "chuen", "eb", "ben", "ix", "mem", "cib", "caban", "eznab", "canac", "ahau"};
        char s[20];
        scanf("%d",&n);
        for(i=0;i<n;i++)
    {
                scanf("%d.%s %d",&d[i],s[i],&y[i]);
            
    }
        for(i=0;i<n;i++)
    {
                sum=0;
                for(j=0;j<=19;j++)
        {
                        if(!strcmp(s[i],haab[j]))
                        break;
                    
        }
                sum = 365* y[i] + j*20+ d[i];
                printf("%d %s %d\n",1 + sum % 13,tzolkin[sum % 20],sum / 260);
       
    }
return 0;
}
