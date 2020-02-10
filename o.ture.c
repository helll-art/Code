/*************************************************************************
	> File Name: tiaoshi.c
	> Author: 
	> Mail: 
	> Created Time: 2020年02月06日 星期四 21时46分18秒
 ************************************************************************/
 #include<stdio.h>
 #include<string.h>
 int panduan(int sum)
 {
     int ret=0,n;
     while(sum>0)
     {
         n=sum%10;
         sum=sum/10;
         ret=n+ret;
     }
     if(ret<10)
        return ret;
     else return panduan(ret);
 }
 int main(void)
 {
     char ch[1001];
     int num;
     while(scanf("%s",ch)!=EOF)
     {
         int sum=0;
         for(int i=0;i<strlen(ch);i++)
         {
             sum=sum+ch[i]-'0';
         }
         if(sum>0)
            printf("%d\n",panduan(sum));
         else return 0;
     }
 }
