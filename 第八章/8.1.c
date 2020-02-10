/*************************************************************************
	> File Name: 8.1.c
	> Author: 
	> Mail: 
	> Created Time: 2020年02月04日 星期二 10时46分17秒
 ************************************************************************/
 #include<stdio.h>
 int main(void)
 {
     int i;
     char ch;
     int cnt=0;
     for(i=0;(ch=getchar())!=EOF;i++)
     {
         if((ch!=' ')&&(ch!='\n')&&(ch!='\t'))
         {
             cnt++;
         }
     }
     printf("字符数：%d\n",cnt);
 }
