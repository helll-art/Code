/*************************************************************************
	> File Name: m.c
	> Author: 
	> Mail: 
	> Created Time: 2020年02月08日 星期六 23时44分19秒
 ************************************************************************/
 #include<stdio.h>
 int main(void)
 {
     long long int n;
     while(scanf("%lld",&n)!=EOF)
     {
         if((n+1)%2==0 || n==0)
            printf("%lld\n",(n+1)/2);
         else printf("%lld\n",n+1);
     }
 }
