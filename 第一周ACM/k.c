/*************************************************************************
	> File Name: k.c
	> Author: 
	> Mail: 
	> Created Time: 2020年02月08日 星期六 22时31分20秒
 ************************************************************************/
 #include<stdio.h>
 int main(void)
 {
     int n,k;
     int i,j;
     char ch[101];
     while(scanf("%d%d",&n,&k)!=EOF)
     {
         i=0;
         j=0;
         while(i<n)
         {
             if(j==k)
                j=0;
             ch[i]='a'+j;
             j++;
             i++;
         }
         ch[n]='\0';
         printf("%s\n",ch);
     }

 }
