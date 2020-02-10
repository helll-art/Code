/*************************************************************************
	> File Name: tiaoshi.c
	> Author: 
	> Mail: 
	> Created Time: 2020年01月31日 星期五 12时25分03秒

 ************************************************************************/
 #include<stdio.h>
 #include<ctype.h>
 int main(void)
 {
     int a[100]={0};
     int cnt=-1;
     double sum=0,average;
     char present,former=' ';
     while((present=getchar())!=EOF)
     {
         if(isalpha(present) && former==' ')
         {
             cnt++;
         }
         if(isalpha(present))
         {
             a[cnt]++;
         }
         former=present;
        
     }
     
     for(int i=0;i<=cnt;i++)
     {
         sum=sum+a[i];
     }
     average=sum/(cnt+1);
     printf("平均字母数：%.2lf\n",average);
     
 }
