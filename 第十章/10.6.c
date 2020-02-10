/*************************************************************************
	> File Name: 10.6.c
	> Author: 
	> Mail: 
	> Created Time: 2020年02月07日 星期五 20时42分16秒
 ************************************************************************/
 #include<stdio.h>
 #define HANG 3
 #define LIE 4
 void copy_2d(double (*source)[LIE], double target[][LIE], int n)
 {
     int i;
     for (i=0;i<n;i++)
     {
        copy_1d(*(source+i),target[i],LIE);
     }
 }
 void copy_1d(double a[],double *b, int n)
 {
     int j;
     for(j=0;j<n;j++)
     {
         *(b+j)=a[j];
     }
 }
 int main(void)
 {
     int i,j;
     double source[HANG][LIE];
     double target[HANG][LIE];
     for( i=0;i<HANG;i++)
     {
         for( j=0;j<LIE;j++)
         {
             scanf("%lf",&source[i][j]);
         }
     }
     copy_2d(source,target,HANG);
     for(i=0;i<HANG;i++)
     {
         for(j=0;j<LIE;j++)
         {
             printf("%.1lf\t",target[i][j]);
         }
         printf("\n");
     }
 }
