/*************************************************************************
	> File Name: 7.3.c
	> Author: 
    > Mail: :w
	> Created Time: 2020年02月01日 星期六 23时53分47秒
 ************************************************************************/
 #include<stdio.h>
 int main(void)
 {
     int a,osum=0,jsum=0;
     double oaverage,javerage,o=0,j=0;
     
     while(1)
     {
         scanf("%d",&a);
         if(a!=0&&a%2==0)
         {
             osum=a+osum;
             o++;
         }
         else if(a!=0 && a%2==1)
         {
             jsum=jsum+a;
             j++;
         }
         else break;
      }
     oaverage=osum/o;
     javerage=jsum/j;
     printf("偶数总个数：%.0lf\n偶数平均值：%.1lf\n奇数总个数：%.0lf\n奇数平均值：%.1lf\n",o,oaverage,j,javerage);
 }

