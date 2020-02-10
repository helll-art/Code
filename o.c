/*************************************************************************
	> File Name: o.c
	> Author: 
	> Mail: 
	> Created Time: 2020年02月09日 星期日 15时27分03秒
 
 ************************************************************************/
 #include<stdio.h>
 long int panduan(long int n)
 {
     long int sum=0;
     int i=0;
     int a[1000]={0};
     while(n>0)
     {
         a[i]=n%10;
         n=n/10;
         i++;
     }
     for(int j=0;j<i;j++)
     {
         sum=sum+a[j];
     }
     if (sum>10)
        return panduan(sum);
     else return sum;
 }
 int main(void)
 {
    long int n;
     while(scanf("%ld",&n)!=EOF)
     {
         if(n!=0)
            printf("%ld\n",panduan(n));
        else return 0;
     }
 }
